@echo off

echo ==================================================================
echo               Building Master Container
echo ==================================================================
echo 
docker-compose up -d master --build


echo ==================================================================
echo               Getting Master container Shell
echo ==================================================================
echo 

docker exec -it dockercl-master-1 bash
