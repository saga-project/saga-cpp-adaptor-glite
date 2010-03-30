#include <saga/saga.hpp>

saga::job::description jd;
jd.set_attribute(saga::job::attributes::description_executable, "/bin/date");

saga::job::service js("cream://cream-09.pd.infn.it:8443/cream-pbs-cream_A");
saga::job::job j = js.create_job(jd);
