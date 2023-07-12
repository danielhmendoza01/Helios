#!/bin/bash
logPath="$1"
mail -s "A mail sent using mailx" dmendoza@tgen.org  < $logPath