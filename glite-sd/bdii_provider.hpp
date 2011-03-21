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
#ifndef INC_BDII_PROVIDER

#define INC_BDII_PROVIDER

#define LDAP_DEPRECATED 1

#define ATTR_NOT_SET "Not Set"
#define ATTR_UID  "GlueServiceUniqueID"
#define ATTR_UID_2  "GLUE2ServiceID"
#define ATTR_EP_UID_2  "GLUE2EndpointID"
#define ATTR_NAME "GlueServiceName"
#define ATTR_NAME_2 "GLUE2EntityName"
#define ATTR_TYPE "GlueServiceType"
#define ATTR_TYPE_2 "GLUE2EndpointInterfaceName"
#define ATTR_URL  "GlueServiceEndpoint"
#define ATTR_URL_2  "GLUE2EndpointURL"
#define ATTR_LINK "GlueForeignKey"
#define ATTR_LINK_2 "GLUE2ServiceServiceForeignKey"
#define ATTR_SITE "GlueSiteUniqueID"
#define ATTR_SITE_2 "GLUE2ServiceAdminDomainForeignKey"
#define ATTR_VO          "GlueServiceOwner"
#define ATTR_ACCESSRULE  "GlueServiceAccessControlRule"
#define ATTR_IMPLEMENTOR_2  "GLUE2EndpointImplementor"
#define ATTR_IMPL_VERSION  "GlueServiceVersion"
#define ATTR_IMPL_VERSION_2  "GLUE2EndpointImplementationVersion"
#define ATTR_INT_VERSION  "GlueServiceVersion"
#define ATTR_INT_VERSION_2  "GLUE2EndpointInterfaceVersion"

//No capability in GLUE 1.3
#define ATTR_CAPABILITY  ""
#define ATTR_CAPABILITY_2  "GLUE2EndpointCapability"

#define ATTR_SA_ROOT         "GlueSARoot"
#define ATTR_SA_ACBR         "GlueSAAccessControlBaseRule"

#define GLITE_GLUE_2_ACCESS_POLICY_SCHEME "org.glite.standard"

#include "saga/impl/packages/sd/info_provider.hpp"

namespace glite_sd_adaptor {
class discoverer_cpi_impl;
}

namespace glite_sd_adaptor {
class bdii_provider: public saga::impl::info_provider {
    private:
        friend class glite_sd_adaptor::discoverer_cpi_impl;

        saga::url server_url;

        static const char *glueAttrib[17];
        static const char *glue2Attrib[5];
        static const char *glue2EPAttrib[11];
        static const char *allAttrib[2];
        static const char *saAttrib[3];
        map<const char*, std::string> attr_map;

        /* LDAP variables */
        LDAP *ld;
        LDAPMessage *searchResult;
        static const char *basedn_glue1;
        static const char *basedn_glue2;
        void list_services(std::string service_filter, std::string data_filter, std::string vo_filter, std::vector<
                saga::sd::service_description> &gsvcs, bool doGlue1, bool doGlue2);

        void list_services_glue1(std::string service_filter, std::string data_filter, std::string vo_filter,
                std::vector<saga::sd::service_description> &gsvcs);

        void list_services_glue2(std::string service_filter, std::string data_filter, std::string vo_filter,
                std::vector<saga::sd::service_description> &gsvcs);

        void get_authz_attributes(const saga::sd::service_description& sd, bool& allFlag, std::set<std::string>& voSet,
                std::set<std::string>& vomsSet, std::set<std::string>& fqanSet, std::set<std::string>& dnSet) const;

        void get_service_data(saga::sd::service_description *svc, const std::string& serviceId,
                const std::string& model);
        std::string get_hostname(std::string endpoint);
        void set_scalar_attribute(saga::sd::service_description* svc, LDAPMessage* e, const char* attr,
                const std::string& defaultStr = "");
        void set_link_attributes_glue1(saga::sd::service_description *sd, LDAPMessage *e);

        std::string get_glue2_site(const std::string& adminDomainId);

        void set_vo_attribute(saga::sd::service_description *sd, LDAPMessage *e);

        /* LDAP specific functions */
        void ldap_connect();
        void ldap_disconnect();
        //void ldap_search(std::string search_filter, std::string data_filter, std::vector<service_description> &vec);
        void ldap_debug_print();

        //Data attribute stuff
        typedef std::multimap<std::string, std::pair<std::string, std::string> > SERVICE_DATA_MM;

        typedef std::multimap<std::string, std::pair<std::string, std::string> >::iterator SERVICE_DATA_MM_ITER;

        SERVICE_DATA_MM data_map;
        SERVICE_DATA_MM mount_map;

        void populate_service_data_glue1();
        void populate_service_data_glue2();

    public:
        bdii_provider(const std::string& url = "");
        ~bdii_provider();
};
}
#endif
