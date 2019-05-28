#include "exception_handler.h"

#include <string>
#include <iostream>
#include <csignal>

#ifdef HAVE_EXECINFO_H
#include <execinfo.h>  // To get stack trace in error messages.
// If this #include fails there is an error in the Makefile, it does not
// support your platform well. Make sure HAVE_EXECINFO_H is undefined,
// and the code will compile.
#ifdef HAVE_CXXABI_H
#include <cxxabi.h>  // For name demangling.
// Useful to decode the stack trace, but only used if we have execinfo.h
#endif  // HAVE_CXXABI_H
#endif  // HAVE_EXECINFO_H

namespace {

static std::string Demangle(std::string trace_name) {
#if defined(HAVE_CXXABI_H) && defined(HAVE_EXECINFO_H)
  // at input the string looks like:
  //   ./kaldi-error-test(_ZN5kaldi13UnitTestErrorEv+0xb) [0x804965d]
  // We want to extract the name e.g. '_ZN5kaldi13UnitTestErrorEv",
  // demangle it and return it.

  // try to locate '(' and '+', take the string in between,
  size_t begin(trace_name.find("(")),
         end(trace_name.rfind("+"));
  if (begin != std::string::npos && end != std::string::npos && begin < end) {
    trace_name = trace_name.substr(begin+1,end-(begin+1));
  }
  // demangle,
  int status;
  char *demangled_name = abi::__cxa_demangle(trace_name.c_str(), 0, 0, &status);
  std::string ans;
  if (status == 0) {
    ans = demangled_name;
    free(demangled_name);
  } else {
    ans = trace_name;
  }
  // return,
  return ans;
#else
  return trace_name;
#endif
}

void Handler(int sig) {
    using std::cout;
    using std::endl;
    cout << "Caught " << sig << endl;
    #ifdef HAVE_EXECINFO_H
    #define KALDI_MAX_TRACE_SIZE 100
    #define KALDI_MAX_TRACE_PRINT 50  // must be even.

    // buffer for the trace,
    void *trace[KALDI_MAX_TRACE_SIZE];
    // get the trace,
    size_t size = backtrace(trace, KALDI_MAX_TRACE_SIZE);
    // get the trace symbols,
    char **trace_symbol = backtrace_symbols(trace, size);

    // Compose the 'string',
    cout << "[ Stack-Trace: ]" << endl;
    if (size <= KALDI_MAX_TRACE_PRINT) {
        for (size_t i = 0; i < size; i++) {
            cout << Demangle(trace_symbol[i]) << endl;
        }
    } else {  // print out first+last (e.g.) 5.
        for (size_t i = 0; i < KALDI_MAX_TRACE_PRINT/2; i++) {
            cout << Demangle(trace_symbol[i]) << endl;
        }
        cout << endl << endl;
        for (size_t i = size - KALDI_MAX_TRACE_PRINT/2; i < size; i++) {
            cout << Demangle(trace_symbol[i]) << endl;
        }
        if (size == KALDI_MAX_TRACE_SIZE)
            cout << endl << endl;  // stack was too long, probably a bug.
    }

    // cleanup,
    free(trace_symbol);  // it's okay, just the pointers, not the strings.
    #endif  // HAVE_EXECINFO_H
    exit(1);
}

}  // namespace

namespace strings {

void InstallHandler() {
    signal(SIGSEGV, Handler);
}

}  // namespace strings

