/*
 * (C) Copyright 2020
 * Zimplistic Private Limited
 * Sicris Rey Embay, sicris@zimplistic.com
 */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 256K
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 48K
}

_HeapSize = 0;
_StackSize = 0x400;