import saga

try: 
  c1 = saga.context("x509")
  c1.set_attribute("UserProxy", "/tmp/x509up_u500_cern")
  
  c3 = saga.context("x509")
  c3.set_attribute("UserProxy", "/does/not/exist")
  
  s = saga.session()
  s.add_context(c1)
  s.add_context(c3)
  
  js = saga.job.service(s, saga.url("cream://cream-09.pd.infn.it:8443/cream-pbs-cream_A"))
  

except saga.exception, e:
  print e.get_full_message()
