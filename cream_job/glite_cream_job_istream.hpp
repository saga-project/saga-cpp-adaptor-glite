//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ADAPTORS_GLITE_CREAM_JOB_ISTREAM_HPP)
#define ADAPTORS_GLITE_CREAM_JOB_ISTREAM_HPP

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga engine includes
#include <saga/impl/job.hpp>

// adaptor includes
#include "glite_cream_job_stream.hpp"


////////////////////////////////////////////////////////////////////////
namespace glite_cream_job 
{
  class istream : public saga::job::istream
  {
    private:
      typedef impl::glite_cream_job::stream <saga::adaptors::istream_ptr> 
              impl_type;

    public:
      template <typename Stream>
      istream (saga::impl::v1_0::job_cpi * cpi, 
               Stream                    & child_istream)
        : saga::job::istream (new impl_type (cpi, child_istream.rdbuf ()))
      {
      }
  };

} // namespace glite_cream_job
////////////////////////////////////////////////////////////////////////

#endif // !defined(ADAPTORS_GLITE_CREAM_JOB_ISTREAM_HPP)

