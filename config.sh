#echo -e 'IP:192.168.1.1' > ./config

#very quick, dirty and ugly ;)
#find the default gateway and set it as dst for the first test
echo -e -n 'IP:' > ./config
route | grep default | cut -c17-32 | tr -s ' ' '\n' >> ./config
echo -e 'ProxyPort:8080' >> ./config

