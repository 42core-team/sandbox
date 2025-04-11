#!/bin/sh

if [ -z "$1" ]; then
  echo "Usage: $0 <number>"
  exit 1
fi

echo "$1" > /core/tickspeed.txt