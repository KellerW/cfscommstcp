docker network create --subnet=172.18.0.0/16 skynet
docker run --net mynet123 --ip 172.18.0.22 -it ubuntu bash