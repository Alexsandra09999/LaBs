#!/bin/bash
set -e

git checkout prd
git merge stg
TAG="release-$(date +%Y%m%d-%H%M%S)"
git tag $TAG
git push origin prd --tags
echo "Перенос из stg в prd выполнен, тег: $TAG"

