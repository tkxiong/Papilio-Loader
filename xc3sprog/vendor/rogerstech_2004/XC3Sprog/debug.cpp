/* JTAG debugging code

Copyright (C) 2004 Andrew Rogers

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */



#include <stdio.h>

#include "iobase.h"
#include "ioparport.h"
#include "iodebug.h"


using namespace std;

void testPP();
void testDebug();
void printBit(unsigned char *data, int bit);
void getSwitches(IOBase *io);
void getID(IOBase *io);

int main(int argc, char**args)
{
  testPP();
  return 0;
}

void testDebug()
{
  IOBase *io;
  io=new IODebug();
  unsigned char tdi[]={0x3a,0xa3};
  unsigned char tdo[10];
  io->setTapState(IOBase::SHIFT_DR);
  io->shiftTDITDO(tdi,tdo,16,false);
  for(int i=0; i<2; i++)printf("TDO %02x\n",tdo[i]);
  delete io;
}

void testPP()
{
  IOBase *io;
  io=new IOParport("/dev/parport0");
  unsigned char tdi[]={0,0,0,0,0,0,0,0};
  unsigned char tdo[100];
  io->setTapState(IOBase::SHIFT_DR);
  io->shiftTDITDO(tdi,tdo,64);
  for(int i=0; i<8; i++)printf("TDO %02x\n",tdo[i]);
  printf("\n");
  getSwitches(io);
  getID(io);
  delete io;
}

void printBit1(bool val)
{
  if(val)printf("|=| ");
  else printf("| | ");
}

void getID(IOBase *io)
{
  unsigned char tdo[100];
  unsigned char tdi[]={0x09,0xff};
  io->setTapState(IOBase::SHIFT_IR);
  io->shiftTDI(tdi,14);
  io->setTapState(IOBase::RUN_TEST_IDLE);
  io->setTapState(IOBase::SHIFT_DR);
  io->shiftTDO(tdo,64);
  for(int i=0; i<8; i++)printf("TDO %02x\n",tdo[i]);
  printf("\n");
}

void getSwitches(IOBase *io)
{
  unsigned char tdo[100];
  unsigned char tdi[]={0x01,0xff};
  io->setTapState(IOBase::SHIFT_IR);
  io->shiftTDI(tdi,14);
  io->setTapState(IOBase::SHIFT_DR);
  io->shiftTDO(tdo,600);
  int swi[]={25,45,56,59,73,86,93,90};
  for(int i=7; i>=0; i--){
    int bit=swi[i];
    bool val=(tdo[bit/8]>>(bit%8))&1;
    printBit1(val);
  }
  printf("\n");
  for(int i=7; i>=0; i--){
    int bit=swi[i];
    bool val=(tdo[bit/8]>>(bit%8))&1;
    printBit1(!val);
  }
  printf("\n\n");
}

void printBit(unsigned char *data, int bit)
{
  printf("bit %d = %d\n",bit,(data[bit/8]>>(bit%8))&1);
}


