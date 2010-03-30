#include <saga/saga.hpp>


int main(int argc, char **argv)
{

  saga::job::description jd;

  jd.set_attribute (saga::job::attributes::description_executable, "/bin/date/");

  saga::job::service js("cream://cream-09.pd.infn.it:8443/cream-pbs-cream_A");
  saga::job::job cream_job = js.create_job(jd);
 
  cream_job.run(); 
  
  std::cout << "\nJob ID    : " << cream_job.get_job_id() << std::endl;
  std::cout << "Job State : "   << cream_job.get_state() << std::endl;
  
  cream_job.wait(-1.0);         // waits for state change
  
  std::cout << "Job State : "   << cream_job.get_state() << std::endl;
}
