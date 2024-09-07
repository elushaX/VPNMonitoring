netstat -atun | awk '$4 ~ /:2338$/ && $6 == "ESTABLISHED" {print $5}' | awk -F: '{print $1}' | sort | uniq
