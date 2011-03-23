/*
 * Copyright (c) Members of the EGEE Collaboration. 2009-2010.
 * See http://www.eu-egee.org/partners/ for details on the copyright
 * holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <antlr/AST.hpp>

#include <ldap.h>
#include <lber.h>
#include <sys/time.h>

#include "sd_parser_ext.hpp"
#include "sd_lexer.hpp"

#include <saga/adaptors/task.hpp>
#include <saga/adaptors/attribute.hpp>

#include <saga/packages/sd/adaptors/service_description.hpp>

#include <impl/packages/sd/info_provider.hpp>
#include "sd_bdii_query.hpp"
#include "bdii_provider.hpp"

#include <boost/shared_ptr.hpp>

#include <algorithm>

#define SD_DEBUG 0

#include <map>
#include <utility>

using namespace std;
using namespace saga::sd;
using saga::impl::info_provider;

namespace glite_sd_adaptor {

  typedef struct {
    std::string name;
    std::string site;
    std::vector<std::string> relatedServices;
  } GLUE2_SERVICE_ATTRS;

  bdii_provider::bdii_provider(const std::string& url) :
    server_url(url) {
    ldap_connect();

    attr_map[ATTR_UID] = saga::sd::attributes::service_description_uid;
    attr_map[ATTR_UID_2] = saga::sd::attributes::service_description_uid;
    attr_map[ATTR_EP_UID_2] = saga::sd::attributes::service_description_uid;
    attr_map[ATTR_NAME] = saga::sd::attributes::service_description_name;
    attr_map[ATTR_NAME_2] = saga::sd::attributes::service_description_name;
    attr_map[ATTR_TYPE] = saga::sd::attributes::service_description_type;
    attr_map[ATTR_TYPE_2] = saga::sd::attributes::service_description_type;
    attr_map[ATTR_URL] = saga::sd::attributes::service_description_url;
    attr_map[ATTR_URL_2] = saga::sd::attributes::service_description_url;
    attr_map[ATTR_VO] = saga::sd::attributes::service_description_vo;
    attr_map[ATTR_SITE] = saga::sd::attributes::service_description_site;
    attr_map[ATTR_SITE_2] = saga::sd::attributes::service_description_site;
    attr_map[ATTR_LINK] = saga::sd::attributes::service_description_relatedservices;
    attr_map[ATTR_LINK_2] = saga::sd::attributes::service_description_relatedservices;
    attr_map[ATTR_IMPLEMENTOR_2] = saga::sd::attributes::service_description_implementor;
    attr_map[ATTR_IMPL_VERSION] = saga::sd::attributes::service_description_implementation_version;
    attr_map[ATTR_IMPL_VERSION_2] = saga::sd::attributes::service_description_implementation_version;
    attr_map[ATTR_INT_VERSION] = saga::sd::attributes::service_description_interface_version;
    attr_map[ATTR_INT_VERSION_2] = saga::sd::attributes::service_description_interface_version;
    attr_map[ATTR_CAPABILITY] = saga::sd::attributes::service_description_capability;
    attr_map[ATTR_CAPABILITY_2] = saga::sd::attributes::service_description_capability;
    // Need to handle site and related services separately.
  }

  bdii_provider::~bdii_provider() {
    ldap_disconnect();
  }

  void bdii_provider::ldap_connect() {
    char errstr[512];
    int rc;

    //If the url was empty try environment variables or use the top level BDII at CERN
    if (server_url == saga::url()) {
      const char* env = saga::detail::safe_getenv("BDII_URL");
      if (env) {
    server_url = env;
      } else {
    env = saga::detail::safe_getenv("LCG_GFAL_INFOSYS");
    if (env) {
      server_url = "ldap://" + std::string(env);
    } else {
      server_url = "ldap://lcg-bdii.cern.ch:2170";
    }
      }
    }

    //There must be a scheme and hostname of ldap_initialize will seg fault
    if (server_url.get_host() == "" || server_url.get_scheme() == "") {
      SAGA_THROW("URL for SD must include scheme and hostname", saga::IncorrectURL);
    }

    /* STEP 1: LDAP Init */
    rc = ldap_initialize(&ld, server_url.get_string().c_str());
    if (rc != LDAP_SUCCESS) {
      sprintf(const_cast<char *>("ldap_initialize: %s"), ldap_err2string(rc));
      SAGA_THROW(std::string(errstr), saga::NoSuccess);
    }

    /* Use the LDAP_OPT_PROTOCOL_VERSION session preference to specify
       that the client is an LDAPv3 client. */
    int version = LDAP_VERSION3;
    ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &version);

    /* STEP 2: Bind to the server.  The client binds anonymously to the server
       (no DN or credentials are specified). */
    //rc = ldap_sasl_bind_s(ld, NULL, /*LDAP_SASL_SIMPLE*/ "", NULL, NULL, NULL, NULL );
    rc = ldap_simple_bind_s(ld, "", "");
    if (rc != LDAP_SUCCESS) {
      sprintf(errstr, "ldap_sasl_bind_s: %s", ldap_err2string(rc));
      SAGA_THROW(std::string(errstr) + " " + server_url.get_url(), saga::NoSuccess);
    }
  }

  void bdii_provider::ldap_disconnect() {
    ldap_unbind_ext(ld, NULL, NULL);
  }

  void bdii_provider::list_services(string service_filter, string data_filter, string vo_filter, vector<
                    service_description> &vec, bool doGlue1, bool doGlue2) {
    vector<service_description> glue1_vec;
    vector<service_description> glue2_vec;

    if (doGlue1) {
      list_services_glue1(service_filter, data_filter, vo_filter, glue1_vec);
    }

    if (doGlue2) {
      list_services_glue2(service_filter, data_filter, vo_filter, glue2_vec);
    }

    //Add our two vectors together
    vec = glue1_vec;
    vec.reserve(glue1_vec.size() + glue2_vec.size());
    vec.insert(vec.end(), glue2_vec.begin(), glue2_vec.end());

    //Seed random number generator
    //Yes we do this every time and no we shouldn't
    srand(unsigned(time(NULL)));

    //Services should be returned in a random order
    //so shuffle the vector
    std::random_shuffle(vec.begin(), vec.end());
  }

  void bdii_provider::get_authz_attributes(const saga::sd::service_description& sd, bool& allFlag,
                       std::set<std::string>& voSet, std::set<std::string>& vomsSet, std::set<std::string>& fqanSet, std::set<
                       std::string>& dnSet) const {
    //Initialise our results;
    allFlag = false;
    voSet.clear();
    vomsSet.clear();
    fqanSet.clear();
    dnSet.clear();

    //Get this endpoint's ID
    std::string uid = sd.get_attribute(saga::sd::attributes::service_description_uid);

    //Now get all the GLUE2AccessPolicy objects associated with this ID
    struct timeval timeout;
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;

    int rc;

    LDAPMessage* result;
    LDAPMessage* e;

    char errStr[512];
    const char* authzAttribs[3] = { "GLUE2PolicyScheme", "GLUE2PolicyRule", NULL };

    std::string filter = "(&(objectClass=GLUE2AccessPolicy)"
      "(GLUE2AccessPolicyEndpointForeignKey=" + uid + "))";

    //Get all the GlueServiceData entries in the information system
    rc = ldap_search_st(ld, basedn_glue2, LDAP_SCOPE_SUBTREE, filter.c_str(), const_cast<char **>(authzAttribs), 0, &timeout,
            &result);

    if (rc != LDAP_SUCCESS) {
      sprintf(errStr, "ldap_search_ext_s: %s", ldap_err2string(rc));
      SAGA_THROW(errStr, saga::NoSuccess);
    }

    bool haveCorrectScheme = false;

    for (e = ldap_first_entry(ld, result); e != NULL; e = ldap_next_entry(ld, e)) {
      char** values;

      try {
    std::string scheme;
    std::string rule;

    // Get the policy scheme
    values = ldap_get_values(ld, e, "GLUE2PolicyScheme");

    if (values != NULL) {
      scheme = values[0];
      ldap_value_free(values);

      if (scheme != GLITE_GLUE_2_ACCESS_POLICY_SCHEME) {
        //Wrong policy so ignore this entry
        continue;
      }

      haveCorrectScheme = true;
    }

    values = ldap_get_values(ld, e, "GLUE2PolicyRule");

    if (values != NULL) {
      for (unsigned int count = 0; values[count] != NULL; ++count) {
        rule = values[count];

        //rule will be one of the following forms
        //VO:<vo>
        //VOMS:<voms>
        //FQAN:<fqan>
        //DN:<dn>
        //ALL

        size_t pos = rule.find_first_of(':');

        if (pos != std::string::npos) {
          std::string ruleType = rule.substr(0, pos);
          std::string ruleVal = rule.substr(pos + 1);

          //Make the rule type lower case to aid our matching
          std::transform(ruleType.begin(), ruleType.end(), ruleType.begin(),
                 static_cast<int(*)(int)> (std::tolower));

          if (ruleType == "vo") {
        voSet.insert(ruleVal);
        continue;
          }

          if (ruleType == "voms") {
        vomsSet.insert(ruleVal);
        continue;
          }

          if (ruleType == "fqan") {
        fqanSet.insert(ruleVal);
        continue;
          }

          if (ruleType == "dn") {
        dnSet.insert(ruleVal);
        continue;
          }
        }

        //Convert the string to lowercase
        //and see if it matches our ALL specifier
        std::transform(rule.begin(), rule.end(), rule.begin(), static_cast<int(*)(int)> (std::tolower));

        if (rule == "all") {
          allFlag = true;
          continue;
        }

        else {
          //Who knows?
          continue;
        }
      }
      ldap_value_free(values);
      values = NULL;
    }
      }

      catch (saga::exception const & ex) {
    ldap_value_free(values);

#if SD_DEBUG
    cout << ex.get_message() << std::endl;
#endif
    //Not much we can do as we don't want to throw and abort
    //the entire search due to one bad entry in the
    //information provider
    continue;
      }
    }

    //Now that we've looped through all the entries we can
    //free up all the memory
    ldap_msgfree(result);
    result = NULL;

    //If we never found the correct policy scheme then assume ALL
    if (haveCorrectScheme == false) {
      allFlag = true;
    }
  }

  void bdii_provider::populate_service_data_glue1() {
    //Clear our two multimaps
    data_map.clear();
    mount_map.clear();

    boost::regex data_regex(".*GlueServiceUniqueID=([^, ]+).*");
    boost::regex mount_regex(".*GlueSEUniqueID=([^, ]+).*");
    boost::match_results<std::string::const_iterator> what;
    boost::match_flag_type flags = boost::match_default;

    struct timeval timeout;
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;

    int rc;

    LDAPMessage* result;
    LDAPMessage* msg;

    char errStr[512];

    //Get all the GlueServiceData entries in the information system
    rc = ldap_search_st(ld, basedn_glue1, LDAP_SCOPE_SUBTREE, "(objectClass=GlueServiceData)", const_cast<char **>(allAttrib), 0,
            &timeout, &result);

    if (rc != LDAP_SUCCESS) {
      sprintf(errStr, "ldap_search_ext_s: %s", ldap_err2string(rc));
      SAGA_THROW(errStr, saga::NoSuccess);
    }

    //Loop over all the entries and add
    //the key value pairs to our multimap
    for (msg = ldap_first_entry(ld, result); msg; msg = ldap_next_entry(ld, msg)) {
      char** key;
      char** value;

      key = ldap_get_values(ld, msg, "GlueServiceDataKey");
      value = ldap_get_values(ld, msg, "GlueServiceDataValue");

      if (!key || !value) {
    ldap_value_free(key);
    ldap_value_free(value);
    continue;
      }

#if SD_DEBUG
      std::cout << key[0] << " = " << value[0] << std::endl;
#endif

      //Service UID from dn of GlueServiceData entry
      char* dn = ldap_get_dn(ld, msg);
      if (dn != NULL) {
    //We can't use a temporary variable in our call to
    //regex_search() as we need access to it after the call
    //via the what variable
    std::string dnstr(dn);
    if (regex_search(dnstr, what, data_regex, flags) == true) {
      if (what[1].matched) {
        std::string founddn(what[1].first, what[1].second);
        data_map.insert(std::make_pair(founddn, std::make_pair(std::string(*key), std::string(*value))));
      }
    }
      }
      ldap_value_free(key);
      ldap_value_free(value);
      ldap_memfree(dn);
    }
    ldap_msgfree(result);

    //Now do another search for GlueSA for mounts
    rc = ldap_search_st(ld, basedn_glue1, LDAP_SCOPE_SUBTREE, "(objectClass=GlueSA)", const_cast<char **>(saAttrib), 0, &timeout,
            &result);

    if (rc != LDAP_SUCCESS) {
      sprintf(errStr, "ldap_search_ext_s: %s", ldap_err2string(rc));
      SAGA_THROW(errStr, saga::NoSuccess);
    }

    for (msg = ldap_first_entry(ld, result); msg; msg = ldap_next_entry(ld, msg)) {
      char** vo;
      char** root;

      vo = ldap_get_values(ld, msg, ATTR_SA_ACBR);
      root = ldap_get_values(ld, msg, ATTR_SA_ROOT);

      if (!vo || !root) {
    ldap_value_free(vo);
    ldap_value_free(root);
    continue;
      }

#if SD_DEBUG
      std::cout << vo[0] << " = " << root[0] << std::endl;
#endif

      string mountpoint = vo[0];
      mountpoint.append(":SEMountPoint");
      string saroot = root[0];
      string::size_type start = saroot.find(":");

      ldap_value_free(vo);
      ldap_value_free(root);

      if (start == string::npos) {
    start = 0;
      }

      else {
    start = start + 1;
      }

      char* dn = ldap_get_dn(ld, msg);
      if (dn != NULL) {
    //We can't use a temporary variable in our call to
    //regex_search() as we need access to it after the call
    //via the what variable
    std::string dnstr(dn);
    if (regex_search(dnstr, what, mount_regex, flags) == true) {
      if (what[1].matched) {
        std::string founddn(what[1].first, what[1].second);
        mount_map.insert(std::make_pair(get_hostname(founddn), std::make_pair(mountpoint, saroot.substr(
                                                        start))));
      }
    }
      }
      ldap_memfree(dn);
    }
    ldap_msgfree(result);
  }

  void bdii_provider::populate_service_data_glue2() {
    //Clear our two multimaps
    data_map.clear();
    mount_map.clear();

    struct timeval timeout;
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;

    int rc;

    LDAPMessage* result;
    LDAPMessage* msg;

    char errStr[512];

    //Get all the GlueServiceData entries in the information system
    rc = ldap_search_st(ld, basedn_glue2, LDAP_SCOPE_SUBTREE, "(objectClass=GLUE2Extension)", const_cast<char **>(allAttrib), 0,
            &timeout, &result);

    if (rc != LDAP_SUCCESS) {
      sprintf(errStr, "ldap_search_ext_s: %s", ldap_err2string(rc));
      SAGA_THROW(errStr, saga::NoSuccess);
    }

    //Loop over all the entries and add
    //the key value pairs to our multimap
    for (msg = ldap_first_entry(ld, result); msg; msg = ldap_next_entry(ld, msg)) {
      char** serviceId;
      char** key;
      char** value;

      serviceId = ldap_get_values(ld, msg, "GLUE2ExtensionEntityForeignKey");
      key = ldap_get_values(ld, msg, "GLUE2ExtensionKey");
      value = ldap_get_values(ld, msg, "GLUE2ExtensionValue");

      if (!serviceId || !key || !value) {
    ldap_value_free(serviceId);
    ldap_value_free(key);
    ldap_value_free(value);
    continue;
      }

#if SD_DEBUG
      std::cout << serviceId[0]
        << " : "
        << key[0]
        << " = "
        << value[0]
        << std::endl;
#endif

      data_map.insert(std::make_pair(std::string(*serviceId), std::make_pair(std::string(*key), std::string(*value))));
      ldap_value_free(serviceId);
      ldap_value_free(key);
      ldap_value_free(value);
    }
    ldap_msgfree(result);
  }

  void bdii_provider::get_service_data(service_description *svc, const std::string& serviceId, const std::string& model) {
#if SD_DEBUG
    std::cout << "Reading data for " << serviceId << std::endl;
#endif

    //Find results from multimap
    std::pair<SERVICE_DATA_MM_ITER, SERVICE_DATA_MM_ITER> ret;
    ret = data_map.equal_range(serviceId);

    SERVICE_DATA_MM_ITER iter;

    for (iter = ret.first; iter != ret.second; ++iter) {
      std::string key = iter->second.first;
      std::string value = iter->second.second;
      info_provider::set_data(svc, key, value);
    }

    /* if the type is SRM,
     * the service data to be searched in objectClass=GlueSA */
    string svctype = svc->get_attribute(attr_map[ATTR_TYPE]);

    if ((model == "glue1") && (svctype.find("srm") == 0)) {
      if (svc->attribute_exists(attr_map[ATTR_URL])) {
    string endpoint = svc->get_attribute(attr_map[ATTR_URL]);

    ret = mount_map.equal_range(get_hostname(endpoint));

    for (iter = ret.first; iter != ret.second; ++iter) {
      info_provider::set_data(svc, iter->second.first, iter->second.second);
    }
      }
    }
  }

  void bdii_provider::set_scalar_attribute(saga::sd::service_description* sd, LDAPMessage* e, const char* attr,
                       const std::string& defaultStr) {
    char** values;
    string avalue("");

    values = ldap_get_values(ld, e, attr);

    if (values != NULL) {
#if SD_DEBUG
      cout << attr << " = " << values[0] << endl;
#endif
      avalue = values[0];
      ldap_value_free(values);

      //Check for zero length string
      if (avalue.empty() == false) {
    //Nasty HACK alert - Deal with both version attributes
    //being the same in GLUE 1.3
    if (strcmp(attr, ATTR_IMPL_VERSION) == 0) {
      info_provider::set_attribute(sd, saga::sd::attributes::service_description_interface_version, avalue);

      info_provider::set_attribute(sd, saga::sd::attributes::service_description_implementation_version,
                       avalue);
    }

    else {
      info_provider::set_attribute(sd, attr_map[attr], avalue);
    }

    return;
      }
    }
    //If we get here then we need to apply the default
    info_provider::set_attribute(sd, attr_map[attr], defaultStr);
  }

  void bdii_provider::set_link_attributes_glue1(saga::sd::service_description* sd, LDAPMessage* e) {
    vector<string> asvc;
    char** values;

    values = ldap_get_values(ld, e, ATTR_LINK);
    if (values) {
      for (int i = 0; values[i]; i++) {
    string link_attr = string(values[i]);
    string::size_type loc = link_attr.find('=');
    if (loc != string::npos) {
      string link_name = link_attr.substr(0, loc);
      string link_value = link_attr.substr(loc + 1);

      //Unfortunately some information systems have case issues
      //with the site attribute so we do case insensitive
      //comparisons here
      std::transform(link_name.begin(), link_name.end(), link_name.begin(),
             static_cast<int(*)(int)> (std::tolower));

      std::string site_attr = ATTR_SITE;
      std::transform(site_attr.begin(), site_attr.end(), site_attr.begin(),
             static_cast<int(*)(int)> (std::tolower));

      std::string uid_attr = ATTR_UID;
      std::transform(uid_attr.begin(), uid_attr.end(), uid_attr.begin(),
             static_cast<int(*)(int)> (std::tolower));

      if (link_name == site_attr) {
        //site should already have a default value so
        //don't over-write it with nothing
        if (link_value.empty() == false) {
          info_provider::set_attribute(sd, attr_map[ATTR_SITE], link_value);
        }
      }

      else if (link_name == uid_attr) {
        asvc.push_back(link_value);
      }
#if SD_DEBUG
      std::cout << link_name << " = " << link_value << std::endl;
#endif
    }
      }
      ldap_value_free(values);

      if (asvc.size() > 0) {
    info_provider::set_vector_attribute(sd, attr_map[ATTR_LINK], asvc);
      }
    }
  }

  std::string bdii_provider::get_glue2_site(const std::string& adminDomainId) {
    std::string retVal = ATTR_NOT_SET;

    struct timeval timeout;
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;

    int rc;

    LDAPMessage* result;
    LDAPMessage* msg;

    char errStr[512];

    const char* adminDomainAttr[2] = { "GLUE2DomainDescription", NULL };

    //Get all the GlueServiceData entries in the information system
    std::string filter = "(&(objectClass=GLUE2AdminDomain)(GLUE2DomainId=" + adminDomainId + "))";

    rc = ldap_search_st(ld, basedn_glue2, LDAP_SCOPE_SUBTREE, filter.c_str(), const_cast<char **>(adminDomainAttr), 0, &timeout,
            &result);

    if (rc != LDAP_SUCCESS) {
      sprintf(errStr, "ldap_search_ext_s: %s", ldap_err2string(rc));
      SAGA_THROW(errStr, saga::NoSuccess);
    }

#if SD_DEBUG
    if ( ldap_count_entries(ld, result) > 1 )
      {
        std::cerr << "More than 1 GLUE2AdminDomain matches "
          << adminDomainId
          << std::endl;
      }
#endif

    //Get the first matching entry
    msg = ldap_first_entry(ld, result);

    if (msg != NULL) {
      char** values;

      values = ldap_get_values(ld, msg, "GLUE2DomainDescription");

      //Set the site if there is one
      if (values) {
    retVal = values[0];
      }
      ldap_value_free(values);
    }
    ldap_msgfree(result);

    return retVal;
  }

  void bdii_provider::set_vo_attribute(saga::sd::service_description* sd, LDAPMessage* e) {
    vector<string> vos;
    char** values;

    values = ldap_get_values(ld, e, ATTR_VO);
    if (values) {
      for (int i = 0; values[i]; i++) {
    vos.push_back(string(values[i]));
      }
      ldap_value_free(values);
    }

    values = ldap_get_values(ld, e, ATTR_ACCESSRULE);
    if (values) {
      for (int i = 0; values[i]; i++) {
    char* str = values[i];
    //Check for string starting 'VO:'
    if (strncmp("VO:", str, 3) == 0) {
      str += 3;
    }
    vos.push_back(string(str));
      }
      ldap_value_free(values);
    }

    //At this point we want to remove duplicates
    //Sort the vector
    std::sort(vos.begin(), vos.end());

    //Get all the unique elements
    std::vector<std::string>::iterator iter;
    iter = std::unique(vos.begin(), vos.end());

    //Remove duplicates
    vos.erase(iter, vos.end());

    if (vos.size() > 0) {
      info_provider::set_vector_attribute(sd, attr_map[ATTR_VO], vos);
    }
  }

  string bdii_provider::get_hostname(string endpoint) {
    std::string::size_type start, end;

    start = endpoint.find("://");
    if (start == string::npos) {
      start = 0;
    }

    else {
      start = start + 3;
    }

    end = endpoint.find(":", start);

    if (end == string::npos) {
      end = endpoint.find("/", start);
      if (end == string::npos) {
    end = endpoint.length();
      }
    }
    return endpoint.substr(start, end - start);
  }

  void bdii_provider::ldap_debug_print() {
    LDAPMessage* e;
    char** values;
    char* dn;
    char* attr;
    int j, nvalues;
    BerElement* ber;

    for (e = ldap_first_entry(ld, searchResult); e != NULL; e = ldap_next_entry(ld, e)) {
      if ((dn = ldap_get_dn(ld, e)) != NULL) {
    printf("DN = %s\n", dn);
    for (attr = ldap_first_attribute(ld, e, &ber); attr != NULL; attr = ldap_next_attribute(ld, e, ber)) {
      values = ldap_get_values(ld, e, attr);
      nvalues = ldap_count_values(values);
      for (j = 0; j < nvalues; j++) {
        printf("%s = %s\n", attr, values[j]);
      }
      ldap_value_free(values);
      ldap_memfree(attr);
    }
    ldap_memfree(dn);
      }
    }
  }

  void bdii_provider::list_services_glue1(string service_filter, string data_filter, string vo_filter, vector<
                      service_description> &vec) {
    LDAPMessage *e;
    char *dn;
    struct timeval timeout;
    int rc;
    bool selectService;
    sd_bdii_query bq(1);
    string final_filter = "(&(objectClass=GlueService)";
    ASTFactory ast_factory;
    bool bothEmpty = true;

    //Empty our results vector
    vec.clear();

    if (!service_filter.empty()) {
      try {
    bothEmpty = false;
    istringstream svc_stream(service_filter, istringstream::in);
    sd_lexer svc_lexer(svc_stream);
    sd_parser_ext svc_parser(svc_lexer);
    svc_parser.initializeASTFactory(ast_factory);
    svc_parser.setASTFactory(&ast_factory);
    svc_parser.service_filter();

    if ( svc_parser.getErrorFlag() == true ) {
      SAGA_THROW(std::string("In Service Filter, " +
                 svc_parser.getErrorString()),
             saga::BadParameter);
    }

    RefAST svc_tree = svc_parser.getAST();
    bq.initialize();

    string svc_filter;
    try {
      svc_filter = bq.get_svc_filter(svc_tree);
    } catch ( saga::exception& e ) {
      std::string msg = e.get_message();

      //Rethrow any exception, unless it was for an unknown capability
      //which we silently ignore.
      //This is to simulate the GLUE1 query failing without
      //compromising GLUE2
      if ( msg.find("Unknown Capability") == std::string::npos ) {
        throw;
      } else {
        return;
      }
    }
    final_filter.append(svc_filter);
      } catch (ANTLR_USE_NAMESPACE(antlr)ANTLRException& e) {
    SAGA_THROW(std::string("In Service Filter, " +
                   e.getMessage()),
           saga::BadParameter);
      }
    }

    if ( !vo_filter.empty() ) {
      try {
    //cout << "VO filter not empty " << endl;
    bothEmpty = false;
    istringstream vo_stream(vo_filter, istringstream::in);
    sd_lexer vo_lexer(vo_stream);
    sd_parser_ext vo_parser(vo_lexer);
    vo_parser.initializeASTFactory(ast_factory);
    vo_parser.setASTFactory(&ast_factory);
    vo_parser.vo_filter();

    if ( vo_parser.getErrorFlag() == true ) {
      SAGA_THROW(std::string("In Authz filter, " +
                 vo_parser.getErrorString()),
             saga::BadParameter);
    }

    RefAST vo_tree = vo_parser.getAST();
    bq.initialize();
    string vo_filter = bq.get_authz_filter(vo_tree);
    final_filter.append(vo_filter);
      } catch (ANTLR_USE_NAMESPACE(antlr)ANTLRException& e) {
    SAGA_THROW(std::string("In Authz Filter, " + e.getMessage()),
           saga::BadParameter);
      }
    }

    if ( bothEmpty ) {
      final_filter.assign("(objectClass=GlueService)");
    } else {
      final_filter.append(")");
    }

#if SD_DEBUG
    cout << final_filter <<endl;
#endif

    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    rc = ldap_search_st(ld,
            const_cast<char *>(basedn_glue1),
            LDAP_SCOPE_SUBTREE,
            final_filter.c_str(),
            const_cast<char **>(glueAttrib),
            0,
            &timeout,
            &searchResult);

    if (rc == LDAP_NO_SUCH_OBJECT) {
      return; // If the server does not know about this point in the tree
    }

    if ( rc != LDAP_SUCCESS ) {
      //Handle this error
      char errstr[512];
      sprintf(errstr, "ldap_search_st: %s", ldap_err2string(rc));
      SAGA_THROW(std::string(errstr), saga::NoSuccess);
    }

    //Now populate the service data multimaps
    //that we use for get_service_data()
    populate_service_data_glue1();

    // If data filter is not empty then evaluate expression
    sd_bdii_query bqdata(1);
    bqdata.initialize();
    RefAST data_tree;

    if ( !data_filter.empty() ) {
      try {
    istringstream data_stream(data_filter, istringstream::in);
    sd_lexer data_lexer(data_stream);
    sd_parser_ext data_parser(data_lexer);
    data_parser.initializeASTFactory(ast_factory);
    data_parser.setASTFactory(&ast_factory);
    data_parser.data_filter();

    if ( data_parser.getErrorFlag() == true ) {
      SAGA_THROW(std::string("In Data Filter, " +
                 data_parser.getErrorString()),
             saga::BadParameter);
    }

    data_tree = data_parser.getAST();
      } catch (ANTLR_USE_NAMESPACE(antlr)ANTLRException& e) {
    SAGA_THROW(std::string("In Data Filter, " +
                   e.getMessage()),
           saga::BadParameter);
      }
    }

    for ( e = ldap_first_entry(ld, searchResult); e != NULL; e = ldap_next_entry(ld, e)) {
      selectService = false;

      if ( (dn = ldap_get_dn( ld, e )) != NULL ) {
    saga::adaptors::service_description local_svc(server_url);
    set_scalar_attribute(&local_svc, e, ATTR_URL);
    set_scalar_attribute(&local_svc, e, ATTR_UID);
    set_scalar_attribute(&local_svc, e, ATTR_IMPL_VERSION);
    set_scalar_attribute(&local_svc, e, ATTR_INT_VERSION);
    set_scalar_attribute(&local_svc, e, ATTR_NAME, ATTR_NOT_SET);
    set_scalar_attribute(&local_svc, e, ATTR_TYPE, ATTR_NOT_SET);

    //This next line sets 'site' to 'Not Set'
    //It might be changed later by set_link_attributes_glue1()
    set_scalar_attribute(&local_svc, e, ATTR_SITE, ATTR_NOT_SET);

    set_link_attributes_glue1(&local_svc, e);
    //set_vo_attribute(&local_svc, e);
    ldap_memfree( dn );

    //Check service has a UID and an URL
    if ( (local_svc.attribute_exists(attr_map[ATTR_UID]) == false) ||
         (local_svc.get_attribute(attr_map[ATTR_UID]).empty() == true) ) {
      continue;
    }

    if ( (local_svc.attribute_exists(attr_map[ATTR_URL]) == false) ||
         (local_svc.get_attribute(attr_map[ATTR_URL]).empty() == true) ) {
      continue;
    }

    // Get service data
    try {
      string serviceId;

      if ( local_svc.attribute_exists(attr_map[ATTR_UID]) ) {
        serviceId = local_svc.get_attribute(attr_map[ATTR_UID]);
      } else {
        SAGA_THROW_NO_OBJECT("No UID in service", saga::NoSuccess);
      }

      get_service_data(&local_svc, serviceId, "glue1");
    } catch ( saga::exception const & ex ) {
#if SD_DEBUG
      cout << ex.get_message() << std::endl;
#endif
      //Not much we can do as we don't want to throw and abort
      //the entire search due to one bad entry in the
      //information provider
      continue;
    }

    // If data filter is not empty then evaluate expression
    if ( !data_filter.empty() ) {
      saga::sd::service_data data = local_svc.get_data();
      selectService = bqdata.evaluate_data_filter(data_tree, data);
    } else {
      //No data filter so just accept this service
      selectService = true;
    }

    if ( selectService ) {
      //local_svc.set_provider(this);
      vec.push_back(local_svc);
    }
    //Free up allocated memory
    //delete local_svc;
      }
    }

    //Now that we've looped through all the entries we can
    //free up all the memory
    ldap_msgfree(searchResult);
    searchResult = NULL;
  }

  void bdii_provider::list_services_glue2(string service_filter,
                      string data_filter,
                      string authz_filter,
                      vector<service_description> &vec) {

    LDAPMessage *e;
    struct timeval timeout;
    int rc;
    bool selectService;
    sd_bdii_query bq(2);
    string final_filter = "(objectClass=GLUE2Service)";
    ASTFactory ast_factory;

    //Create a map to store our service details in
    //Map of service ID to Name
    std::map<std::string, GLUE2_SERVICE_ATTRS> serviceMap;

    // If data filter is not empty then evaluate expression
    sd_bdii_query bqsvc(2);
    sd_bdii_query bqauthz(2);
    sd_bdii_query bqdata(2);
    bqsvc.initialize();
    bqauthz.initialize();
    bqdata.initialize();
    RefAST svc_tree;
    RefAST authz_tree;
    RefAST data_tree;

    if ( !service_filter.empty() ) {
      try {
    istringstream svc_stream(service_filter, istringstream::in);
    sd_lexer svc_lexer(svc_stream);
    sd_parser_ext svc_parser(svc_lexer);
    svc_parser.initializeASTFactory(ast_factory);
    svc_parser.setASTFactory(&ast_factory);
    svc_parser.service_filter();

    if ( svc_parser.getErrorFlag() == true )
      {
        SAGA_THROW(std::string("In Service Filter, " +
                   svc_parser.getErrorString()),
               saga::BadParameter);
      }

    svc_tree = svc_parser.getAST();
      } catch (ANTLR_USE_NAMESPACE(antlr)ANTLRException& e) {
    SAGA_THROW(std::string("In Service Filter, " + e.getMessage()),
           saga::BadParameter);
      }
    }

    if ( !authz_filter.empty() ) {
      try {
    istringstream authz_stream(authz_filter, istringstream::in);
    sd_lexer authz_lexer(authz_stream);
    sd_parser_ext authz_parser(authz_lexer);
    authz_parser.initializeASTFactory(ast_factory);
    authz_parser.setASTFactory(&ast_factory);
    authz_parser.vo_filter();

    if ( authz_parser.getErrorFlag() == true ) {
      SAGA_THROW(std::string("In Authz Filter, " +
                 authz_parser.getErrorString()),
             saga::BadParameter);
    }

    authz_tree = authz_parser.getAST();
      } catch (ANTLR_USE_NAMESPACE(antlr)ANTLRException& e) {
    SAGA_THROW(std::string("In Service Filter, " +
                   e.getMessage()),
           saga::BadParameter);
      }
    }

    if ( !data_filter.empty() ) {
      try {
    istringstream data_stream(data_filter, istringstream::in);
    sd_lexer data_lexer(data_stream);
    sd_parser_ext data_parser(data_lexer);
    data_parser.initializeASTFactory(ast_factory);
    data_parser.setASTFactory(&ast_factory);
    data_parser.data_filter();

    if ( data_parser.getErrorFlag() == true ) {
      SAGA_THROW(std::string("In Data Filter, " +
                 data_parser.getErrorString()),
             saga::BadParameter);
    }

    data_tree = data_parser.getAST();
      } catch (ANTLR_USE_NAMESPACE(antlr)ANTLRException& e) {
    SAGA_THROW(std::string("In Data Filter, " +
                   e.getMessage()),
           saga::BadParameter);
      }
    }

    //With GLUE2 we have to get all services
    //and then do our filtering afterwards
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    rc = ldap_search_st(ld,
            const_cast<char *>(basedn_glue2),
            LDAP_SCOPE_SUBTREE,
            final_filter.c_str(),
            const_cast<char **>(glue2Attrib),
            0,
            &timeout,
            &searchResult);

    if (rc == LDAP_NO_SUCH_OBJECT) {
      return; // If the server does not know about this point in the tree
    }

    if ( rc != LDAP_SUCCESS ) {
      //Handle this error
      char errstr[512];
      sprintf(errstr, "ldap_search_st: %s", ldap_err2string(rc));
      SAGA_THROW(std::string(errstr), saga::NoSuccess);
    }

    //Now populate the service data multimaps
    //that we use for get_service_data()
    populate_service_data_glue2();

    for ( e = ldap_first_entry(ld, searchResult); e != NULL; e = ldap_next_entry(ld, e)) {
      selectService = false;

      // Get Service ID and name
      try {
    char** values;
    std::string serviceId;
    std::string serviceName;
    std::string adminDomainId;
    std::string serviceSite;
    std::vector<std::string> relatedServices;

    values = ldap_get_values(ld, e, ATTR_UID_2);

    if ( values != NULL ) {
      serviceId = values[0];
      ldap_value_free(values);
    }

    values = ldap_get_values(ld, e, ATTR_NAME_2);

    if ( values != NULL ) {
      serviceName = values[0];
      ldap_value_free(values);
    }

    //Get the site
    serviceSite = ATTR_NOT_SET;
    values = ldap_get_values(ld, e, ATTR_SITE_2);

    if ( values != NULL ) {
      adminDomainId = values[0];
      ldap_value_free(values);
      serviceSite = get_glue2_site(adminDomainId);
    }

    //Get the related services
    values = ldap_get_values(ld, e, ATTR_LINK_2);

    if ( values != NULL ) {
      unsigned int count = 0;
      char* value = values[count];
      while ( value != NULL ) {
        relatedServices.push_back(std::string(value));
        ++count;
        value = values[count];
      }
      ldap_value_free(values);
    }

    //Add this service to our map
    //if there's no service ID then just ignore
    if ( serviceId.empty() == false ) {
      GLUE2_SERVICE_ATTRS attrs;
      attrs.name = serviceName;
      attrs.site = serviceSite;
      attrs.relatedServices = relatedServices;
      serviceMap.insert(std::make_pair(serviceId, attrs));
    }
      } catch ( saga::exception const & ex ) {
#if SD_DEBUG
    cout << ex.get_message() << std::endl;
#endif
    //Not much we can do as we don't want to throw and abort
    //the entire search due to one bad entry in the
    //information provider
    continue;
      }
    }

    //Now that we've looped through all the entries we can
    //free up all the memory
    ldap_msgfree(searchResult);
    searchResult = NULL;

    //With GLUE2 we have to get all the endpoints
    //and then do our filtering afterwards
    final_filter = "(objectClass=GLUE2Endpoint)";
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    rc = ldap_search_st(ld,
            basedn_glue2,
            LDAP_SCOPE_SUBTREE,
            final_filter.c_str(),
            const_cast<char **>(glue2EPAttrib),
            0,
            &timeout,
            &searchResult);

    if ( rc != LDAP_SUCCESS ) {
      //Handle this error
      char errstr[512];
      sprintf(errstr, "ldap_search_st: %s", ldap_err2string(rc));
      SAGA_THROW(std::string(errstr), saga::NoSuccess);
    }

    for ( e = ldap_first_entry(ld, searchResult); e != NULL; e = ldap_next_entry(ld, e)) {
      saga::adaptors::service_description local_svc(server_url);
      selectService = false;

      // Get service endpoint
      try {
    char** values;
    std::string serviceId;
    values = ldap_get_values(ld, e, "GLUE2EndpointServiceForeignKey");
    if ( values != NULL ) {

#if SD_DEBUG
      cout << "ServiceID for EP = " << values[0] << endl;
#endif
      serviceId = values[0];
      ldap_value_free(values);
    }

    if ( serviceId.empty() == false )
      {
        //Find this service in our map
        std::map<std::string, GLUE2_SERVICE_ATTRS>::const_iterator
          iter;

        iter = serviceMap.find(serviceId);

        if ( iter != serviceMap.end() )
          {
        GLUE2_SERVICE_ATTRS srvAttr = iter->second;

        //Set the service name
        info_provider::set_attribute(&local_svc,
                         attr_map[ATTR_NAME_2],
                         srvAttr.name);

        //Set the service site
        info_provider::set_attribute(&local_svc,
                         attr_map[ATTR_SITE_2],
                         srvAttr.site);

        //Set the related services
        info_provider::set_vector_attribute(&local_svc,
                            attr_map[ATTR_LINK_2],
                            srvAttr.relatedServices);
          }

        else
          {
        //No service for this enpoint
        //Not our problem so just continue
        continue;
          }
      }

    else
      {
        //No Service ID co carry on to the next Endpoint
        continue;
      }

    set_scalar_attribute(&local_svc, e, ATTR_EP_UID_2);
    set_scalar_attribute(&local_svc, e, ATTR_TYPE_2, ATTR_NOT_SET);
    set_scalar_attribute(&local_svc, e, ATTR_URL_2, ATTR_NOT_SET);
    set_scalar_attribute(&local_svc, e, ATTR_IMPLEMENTOR_2, ATTR_NOT_SET);
    set_scalar_attribute(&local_svc, e, ATTR_IMPL_VERSION_2, ATTR_NOT_SET);
    set_scalar_attribute(&local_svc, e, ATTR_INT_VERSION_2, ATTR_NOT_SET);

    //Get capabilities
    values = ldap_get_values(ld, e, ATTR_CAPABILITY_2);

    std::vector<std::string> capabilities;
    if ( values )
      {
        for ( int i=0; values[i]; i++ )
          {
        char* str = values[i];
        capabilities.push_back(string(str));
          }
        ldap_value_free(values);
      }

    //Set the capabilities
    set_vector_attribute(&local_svc,
                 attr_map[ATTR_CAPABILITY_2],
                 capabilities);

    //Check service has a UID and an URL
    if ( (local_svc.attribute_exists(attr_map[ATTR_UID_2]) == false) ||
         (local_svc.get_attribute(attr_map[ATTR_UID_2]).empty() == true) )
      {
        continue;
      }

    if ( (local_svc.attribute_exists(attr_map[ATTR_URL]) == false) ||
         (local_svc.get_attribute(attr_map[ATTR_URL]).empty() == true) )
      {
        continue;
      }
      }

      catch ( saga::exception const & ex )
        {
#if SD_DEBUG
      cout << ex.get_message() << std::endl;
#endif
      //Not much we can do as we don't want to throw and abort
      //the entire search due to one bad entry in the
      //information provider
      continue;
        }

      // Get service data
      try
        {
      char** endpointId;
      std::string epId;

      endpointId = ldap_get_values(ld, e, "GLUE2EndpointID");

      if ( endpointId != NULL )
            {

#if SD_DEBUG
          cout << "ID for EP = " << endpointId[0] << endl;
#endif
          epId = endpointId[0];
          ldap_value_free(endpointId);
            }
      get_service_data(&local_svc, epId, "glue2");
        }

      catch ( saga::exception const & ex )
        {
#if SD_DEBUG
      cout << ex.get_message() << std::endl;
#endif
      //Not much we can do as we don't want to throw and abort
      //the entire search due to one bad entry in the
      //information provider
      continue;
        }

      //Check that this service passes the service filter
      if ( !service_filter.empty() )
        {
      if ( bqsvc.evaluate_service_filter(svc_tree, local_svc) == false )
            {
          //Filter fails so no point in checking the authz or data filter
          //carry on to the next service
          continue;
            }
        }

      //Check that this service passes the authz filter
      if ( !authz_filter.empty() )
        {
      std::set<std::string> voSet;
      std::set<std::string> vomsSet;
      std::set<std::string> fqanSet;
      std::set<std::string> dnSet;
      bool allFlag;

      get_authz_attributes(local_svc,
                   allFlag,
                   voSet,
                   vomsSet,
                   fqanSet,
                   dnSet);

      if ( bqauthz.evaluate_authz_filter(authz_tree,
                         local_svc,
                         allFlag,
                         voSet,
                         vomsSet,
                         fqanSet,
                         dnSet) == false )
            {
          //Filter fails so no point in checking the data filter
          //carry on to the next service
          continue;
            }
        }

      // If data filter is not empty then evaluate expression
      if ( !data_filter.empty() )
        {
      saga::sd::service_data data = local_svc.get_data();
      selectService = bqdata.evaluate_data_filter(data_tree, data);
        }

      else
        {
      //No data filter so just accept this service
      selectService = true;
        }

      if ( selectService )
        {
      //local_svc.set_provider(this);
      vec.push_back(local_svc);
        }
    }

    //Now that we've looped through all the entries we can
    //free up all the memory
    ldap_msgfree(searchResult);
    searchResult = NULL;
  }


  const char* bdii_provider::basedn_glue1 = "o=grid";
  const char* bdii_provider::basedn_glue2 = "o=glue";

  const char* bdii_provider::glueAttrib[17] = { ATTR_UID, ATTR_NAME, ATTR_TYPE, "GlueServiceVersion",
                        "GlueServiceEndpoint", "GlueServiceAccessPointURL", "GlueServiceStatus", "GlueServiceStatusInfo",
                        "GlueServiceWSDL", "GlueServiceSemantics", "GlueServiceStartTime", ATTR_VO, "GlueServiceAccessControlRule",
                        ATTR_LINK, /* = GlueSiteUniqueID=CERN-PROD */
                        "GlueSchemaVersionMajor", "GlueSchemaVersionMinor", NULL };

  const char* bdii_provider::glue2Attrib[5] = { ATTR_UID_2, ATTR_NAME_2, ATTR_LINK_2, ATTR_SITE_2, NULL };

  const char* bdii_provider::glue2EPAttrib[11] = { "GLUE2EndpointID", ATTR_UID_2, ATTR_EP_UID_2, ATTR_TYPE_2,
                           ATTR_IMPLEMENTOR_2, ATTR_IMPL_VERSION_2, ATTR_INT_VERSION_2, ATTR_URL_2, "GLUE2EndpointServiceForeignKey",
                           ATTR_CAPABILITY_2, NULL };

  const char* bdii_provider::allAttrib[2] = { LDAP_ALL_USER_ATTRIBUTES, NULL };

  const char* bdii_provider::saAttrib[3] = { ATTR_SA_ROOT, ATTR_SA_ACBR, NULL };

}
//namespace glite_sd_adaptor
