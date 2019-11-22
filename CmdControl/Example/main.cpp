#include <iostream>
#include "ShellFunSet.h"
using namespace std;

int main(){
  CmdNode<ShellFunSet> shellCmds[9] = {
    {"pwd",&ShellFunSet::pwd},
    {"dir",&ShellFunSet::dir},
    {"cd",&ShellFunSet::cd},
    {"newdir",&ShellFunSet::newdir},
    {"deldir",&ShellFunSet::deldir},
    {"exit",&ShellFunSet::exit},
    {"rename",&ShellFunSet::rename},
    {"find",&ShellFunSet::find},
    {"date",&ShellFunSet::date}
  };
  ShellFunSet osFunHolder;
  CmdControl<ShellFunSet> cmdModul("YeShizhe@",9,osFunHolder,shellCmds);
  cmdModul.run();
  return 0;
}
