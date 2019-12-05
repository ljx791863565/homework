#!/bin/bash

DATE=`date +%d%m%y`
git add --all 
git commit -m "$DATE at company"
git push origin master


