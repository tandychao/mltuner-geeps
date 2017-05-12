import time
import os
import sys
from hyperopt import fmin, tpe, hp

data_file = sys.argv[1]
# data_file = '/users/hengganc/nfs/output/17-0302-1233-test-hyperopt/results.dat'

def my_func(single_input):
  print single_input
  output_fd = open(data_file, 'a')
  input = [single_input]
  for x in input:
    output_fd.write('%f ' % x)
  output_fd.write('P\n')
  output_fd.close()
  while True:
    input_fd = open(data_file, 'r')
    last_line = ''
    for line in input_fd:
      last_line = line
    strs = last_line.split()
    if len(strs) < len(input) + 1 or strs[len(input)] == 'P':
      input_fd.close()
      time.sleep(1)
      continue
    else:
      input_fd.close()
      return float(strs[len(input)])

search_space = (hp.uniform('lr', -5, 0))

os.system('rm %s' % data_file)
best = fmin(fn=my_func,
    space=search_space,
    algo=tpe.suggest,
    max_evals=10000000)
print best
