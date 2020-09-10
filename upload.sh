#!/bin/bash

#curl -H "Content-Type:application/zip" -F ’name=file’ -F ‘filename=@./exports/createOW.zip’ http://192.168.10.124:50000/main/upload
#curl -H "application/zip" --data-binary @exports/createOW.zip -X POST http://192.168.10.124:50000/main/upload
#curl  --header "Accept-Encoding: gzip, deflate" --header "Accept: application/json, text/plain, */*" --request "POST" "http://192.168.10.124:50000/main/upload" --data-binary @exports/createOW.zip
curl --verbose  --header "Accept-Encoding: gzip, deflate" --header "Accept: application/json, text/plain, */*" --header "Connection: keep-alive" --request "POST" "http://192.168.10.124:50000/main/upload" -F "name=file" -F "filename=@exports/createOW.zip"