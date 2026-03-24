//
//  sys.cpp
//  x86
//
//  Created by Chidume Nnamdi on 20/03/2026.
//

#include "sys.hpp"

void handleSyscall(CPU cpu) {
  int syscall = cpu.regs[0]; // EAX

  switch (syscall) {
    case 4: { // write(fd, buf, len)
      int fd = cpu.regs[1];
      int buf = cpu.regs[2];
      int len = cpu.regs[3];

      string output = "";
      for (int i = 0; i < len; i++) {
        output += read8(buf + i);
      }

      cout << (output);
      break;
    }

    case 1: { // exit
      cout << "Program exited" << endl;
//      process.exit(0);
    }
  }
}
