import saga

try: 
  c1 = saga.context("x509")
  c1.set_attribute("UserProxy", "/tmp/x509up_u500_cern")
  
  c2 = saga.context("x509")
  c2.set_attribute("UserProxy", "/tmp/x509up_u500_ncsa")
  
  s = saga.session()
  s.add_context(c1)
  s.add_context(c2)
  
  js = saga.job.service(s, saga.url("gram://qb1.loni.org"))
  

except saga.exception, e:
  print e.get_full_message()
