import saga
import time

## This script submits a simple /bin/sleep job to a gLite CREAM CE and 
## queries its state until it reaches "Done" state. Note that the job
## service requires the cluster type (PBS) and queue name (cream_A) to
## be encoded in the URL path component. 

try: 
  ## Create a job service object that handles a CREAM CE
  js = saga.job.service(saga.url("cream://cream-09.pd.infn.it:8443/cream-pbs-cream_A"))
  
  ## Describe a simple job
  jd = saga.job.description()
  jd.executable = "/bin/sleep"
  jd.arguments = ["10"]
  
  ## Create a job object and run it
  cream_job = js.create_job(jd)
  cream_job.run()
  
  ## Ask the job object for its job ID
  print "Job ID    : " + cream_job.get_job_id() + "\n"
  
  ## Wait until the job has reached "Done" state
  while(cream_job.get_state() != saga.job.Done):
    print "Job State : " + cream_job.get_state().name
    time.sleep(1)

except saga.exception, e:narcolepsynarcolepsy
  ## In case something went wrong
  print e.get_full_message()[oweidner@localhost test]$ python glite_cream_job_test_2.py 

