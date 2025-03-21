#!/bin/bash

docker run --rm -it --mount type=bind,source="$(cygpath -w "$(pwd)")/../src",target=/workspace/src miyooa30-toolchain bash -c "cd /workspace/src && source ~/config.sh && make miyooa30"
docker image prune -f
rm -rf ScoundrelGame.tar.gz
tar -czf ScoundrelGame.tar.gz -C ../src/dist --transform 's,^,ScoundrelGame/,' .