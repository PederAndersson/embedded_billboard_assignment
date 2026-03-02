set pagination off
set print pretty on
set disassemble-next-line on
set breakpoint pending on

handle SIGTRAP stop print nopass
