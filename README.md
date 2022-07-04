# tech-project
docker-compose up --build -d dev-image && docker exec -it tech-project_dev-image_1 /bin/bash
docker-compose up --build -d dev-image ; docker exec -it tech-project_dev-image_1 /bin/bash
docker-compose up --build --scale worker-image=1 worker-image
docker-compose up --build --scale worker-image=3 worker-image
docker-compose up --build worker-image-1 worker-image-2 worker-image-3 worker-image-4 worker-image-5


# kubernetes
- minikube start --insecure-registry true #running the cluster
- kubectl apply -f worker-deployment.yaml #running the deployments


exec kubectl exec -i -t -n default worker-envars-fieldref-statefulset-0 -c tech-project-worker-image-z26m4 "--" sh -c "clear; (bash || ash || sh)"