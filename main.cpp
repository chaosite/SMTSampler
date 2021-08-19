//#include "smtsampler.cpp"
//#include "sampler.h"
#include <cstring>

#include "sampler.h"
#include "megasampler.h"
#include "smtsampler.h"
#include "pythonfuncs.h"

int main(int argc, char *argv[]) {

  int max_epochs = 10;
  int max_samples = 1000000;
  double max_time = 3600.0;
  int max_epoch_samples = 10000;
  double max_epoch_time = 600.0;
  int strategy = STRAT_SMTBIT;
  if (argc < 2) {
    std::cout << "Argument required: input file\n";
    return 0;
  }
  bool arg_samples = false;
  bool arg_time = false;
  bool arg_epoch_samples = false;
  bool arg_epoch_time = false;
  bool arg_num_epochs = false;
  bool use_smtsampler = false;
  bool waits_for_json_dir = false;
  bool json = false;
  std::string json_dir;

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-n") == 0)
      arg_samples = true;
    else if (strcmp(argv[i], "-t") == 0)
      arg_time = true;
    else if (strcmp(argv[i], "-e") == 0)
      arg_num_epochs = true;
    else if (strcmp(argv[i], "-en") == 0)
      arg_epoch_samples = true;
    else if (strcmp(argv[i], "-et") == 0)
      arg_epoch_time = true;
    else if (strcmp(argv[i], "--smtbit") == 0)
      strategy = STRAT_SMTBIT;
    else if (strcmp(argv[i], "--smtbv") == 0)
      strategy = STRAT_SMTBV;
    else if (strcmp(argv[i], "--sat") == 0)
      strategy = STRAT_SAT;
    else if (strcmp(argv[i], "--smtsampler") == 0)
      use_smtsampler = true;
    else if (strcmp(argv[i], "--json") == 0){
    	waits_for_json_dir = true;
    	json = true;
    } else if (arg_samples) {
      arg_samples = false;
      max_samples = atoi(argv[i]);
    } else if (arg_time) {
      arg_time = false;
      max_time = atof(argv[i]);
    } else if (waits_for_json_dir) {
    	waits_for_json_dir = false;
        json_dir = argv[i];
    } else if (arg_epoch_samples) {
      arg_epoch_samples = false;
      max_epoch_samples = atoi(argv[i]);
    } else if (arg_epoch_time) {
      arg_epoch_time = false;
      max_epoch_time = atof(argv[i]);
    } else if (arg_num_epochs) {
      arg_num_epochs = false;
      max_epochs = atof(argv[i]);
    }
  }

  if (strategy == STRAT_SAT) {
    std::cout << "Conversion to SAT is temporarily not supported" << std::endl;
    return 0;
  }

  // initialize_python();
  Sampler * sp;
  if (use_smtsampler){
	  sp = new SMTSampler(argv[argc - 1], max_samples, max_time, max_epoch_samples, max_epoch_time, strategy, json);
  } else {
	  sp = new MEGASampler(argv[argc - 1], max_samples, max_time, max_epoch_samples, max_epoch_time, strategy, json);
  }
  Sampler & s = *sp;
  patch_global_context(s.c);
  s.set_timer_on("total");
  s.set_timer_on("initial_solving");
  s.check_if_satisfiable();
  s.accumulate_time("initial_solving");
  try {
    for (int epochs = 0; epochs < max_epochs; epochs++) {
      s.set_timer_on("start_epoch");
      const z3::model &m = s.start_epoch();
      s.accumulate_time("start_epoch");
      //               std::cout<<m<<std::endl;
      s.set_timer_on("do_epoch");
      s.do_epoch(m);
      s.accumulate_time("do_epoch");
    }
  } catch (z3::exception &except) {
    std::cout << "Termination due to: " << except << "\n";
  }
  s.accumulate_time("total");
  s.safe_exit(0);
  delete sp; //todo: this is unreachable because we exit on safe_exit
  // finalize_python();
  return 0;

  //    double time = get_time_diff(s.get_start_time());
  //    std::cout<<"Time to find initial model: "<<time<<std::endl;
  // s.calculate_coverage_under_model();
  //    s.compute_formula_statistics();
  //    s.print_formula_statistics();
  //    s.run();
}
