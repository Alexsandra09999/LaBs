#!/bin/bash
set -e

git checkout stg
git merge dev
TAG="release-$(date +%Y%m%d-%H%M%S)"
git tag $TAG
git push origin stg --tags
echo "Перенос из dev в stg выполнен, тег: $TAG"

