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
  print "\nJob ID    : " + cream_job.get_job_id() + "\n"
  
  ## Wait until the job has reached "Done" state
  print "Job State : " + cream_job.get_state().name
  cream_job.wait(-1.0) # wait for state change
  print "Job State : " + cream_job.get_state().name
    
except saga.exception, e:
  ## In case something went wrong
  print e.get_full_message() 

