This is a project in 236371 - Implementing Avalanche

Prerequisite:

	1. AWS account
  
	2. K8s cluster up and running in AWS 
  
	3. VPC
  
	4. ECR

How to run the project:

1) run ./build_and_deployment

	The above does the following:
  
		a. build a docker image with the source code
    
		b. tag and push the image to AWS - ECR.
    
		c. apply the changes to the K8S cluster
    
2) wait until the avalanche process is finished

3) run ./download_from_k8s.sh

	the above does the following:
  
		a. downloading the log file from the pods
    
		b. wait until the downloading process is finished
    
		c. run the parser
    
4) the parser script is responsible to calculate the avg run time , % agreement and plot a graph. 

5) All the parameters of the avalanche protocol are defined in worker-deployment.yaml file and injected as environment variable to the pods.
