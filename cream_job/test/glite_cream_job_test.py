import saga

try: 
  c2 = saga.context("x509")
  c2.set_attribute("UserProxy", "/tmp/x509up_u500_cern")
  
  c3 = saga.context("x509")
  c3.set_attribute("UserProxy", "/does/not/exist")
  
  s = saga.session()
  s.add_context(c2)
  s.add_context(c3)
  
  js = saga.job.service(s, saga.url("cream://localhost"))

except saga.exception, e:
  print e.get_full_message()
