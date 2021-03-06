#ifndef __hyperopt_searcher_hpp__
#define __hyperopt_searcher_hpp__

/*
 * Copyright (C) 2013 by Carnegie Mellon University.
 */

#include "mltuner/tunable-searcher.hpp"


class HyperoptSearcher : public TunableSearcher {
  struct TunableExpResult {
    TunableChoice tunable_choice;
    double loss;
    bool exp_done;
    TunableExpResult() : exp_done(false) {}
    TunableExpResult(const TunableChoice& tunable_choice)
        : tunable_choice(tunable_choice), exp_done(false) {}
  };
  typedef vector<TunableExpResult> TunableExpResults;

 public:
  HyperoptSearcher(const Config& config,
      const string& app_name, const string& identity,
      const TunableIds& tunable_ids);
  ~HyperoptSearcher();
  virtual void start();
  virtual void stop();
  virtual bool get_result(TunableChoice *tunable_choice_ptr, uint *exp_id_ptr);
  virtual void set_result(uint exp_id, double cost, double loss);
  virtual double get_searcher_time() {
    return searcher_time_;
  }

 private:
  const Config& config_;
  boost::mutex mutex_;
  boost::condition_variable cvar_;
  string searcher_root_;
  string app_name_;
  string identity_;
  string searcher_cmd_;
  string result_file_;
  TunableExpResults tunable_exp_results_;
  std::set<uint> pending_exp_set_;
  TunableIds tunable_ids_;
  double searcher_time_;
  shared_ptr<boost::thread> background_thread_;

  void init();
  void operator()();
  void background_thread_entry();
  void call_searcher();
  void write_result_file();
  void read_result_file();
  bool check_convergence();
  bool is_topk_close_enough(const DoubleVec& x, int k, double threshold);
  bool is_no_improvement_over_lastk(const DoubleVec& x, int k);
};

#endif  // defined __hyperopt_searcher_hpp__