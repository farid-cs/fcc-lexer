#!/bin/sh
set -xe

./facc '   '
./facc '  // This is a comment'
./facc '  /* This is a block comment */ '
! ./facc ' /*  An errorneous comment /'
! ./facc '     A '
