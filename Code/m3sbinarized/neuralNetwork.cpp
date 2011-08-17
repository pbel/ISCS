#include "stdio.h"

#ifndef _NEURAL_NETWORK_CPP
#define _NEURAL_NETWORK_CPP

#include "neuralNetwork.h"
#include "matrix.h"

extern int	NODES_PER_SENSOR;
extern int	NODES_PER_MOTOR;

NEURAL_NETWORK::NEURAL_NETWORK(	int nS, int nM ) {

	numSensors	= nS;

	numMotors 	= nM;

	Initialize();

	weights		= new MATRIX((NODES_PER_SENSOR*numSensors)+numNodes,numNodes,0);	
//	Weights_Initialize_Randomly(weights);
	Weights_Initialize_NumIncomingConnections(2);

//	printf("%d %d (%d x %d)\n",numSensors,numMotors,numSensors+numNodes,numNodes);
//	weights->Print();
//	exit(0);
}

NEURAL_NETWORK::NEURAL_NETWORK(	NEURAL_NETWORK *other ) {

        weights 	= new MATRIX(other->weights);

	numSensors	= other->numSensors;

	numMotors  	= other->numMotors;

	Initialize();
}

NEURAL_NETWORK::NEURAL_NETWORK(ifstream *inFile) {

	int isWeights;
	(*inFile) >> isWeights;
	if ( isWeights ) {
		weights = new MATRIX(inFile);
	}
	else
		weights = NULL;

	(*inFile) >> numSensors;

        (*inFile) >> numMotors;

	Initialize();
}

NEURAL_NETWORK::~NEURAL_NETWORK(void) {

	if ( weights ) {
		delete weights;
		weights = NULL;
	}

	if ( nodeValues ) {
		delete nodeValues;
		nodeValues = NULL;
	}

	if ( temp ) {
		delete temp;
		temp = NULL;
	}

	if ( edgeList ) {
		delete edgeList;
		edgeList = NULL;
	}

	if ( sensorValues ) {
		delete sensorValues;
		sensorValues = NULL;
	}
}

double NEURAL_NETWORK::Density(void) {

	double density = 0.0;

	for (int j=0;	j<numNodes;	j++)

		for (int i=0;	i<((NODES_PER_SENSOR*numSensors)+numNodes);	i++)

			if ( weights->Get(i,j)!=0 )

				density++;

	density = density / double(((NODES_PER_SENSOR*numSensors)+numNodes)*numNodes);

	return( density );
}

double NEURAL_NETWORK::Get_Motor_Neuron_Value(int motorIndex) {

	int firstBit =  nodeValues->vals[NODES_PER_MOTOR*motorIndex];
	if ( firstBit==-1 )
		firstBit=0;

	int secondBit = nodeValues->vals[NODES_PER_MOTOR*motorIndex+1];
	if ( secondBit==-1 )
		secondBit=0;

	// Uses gray codes
	double rawValue;
	     if ( (firstBit==0) && (secondBit==0) )
		rawValue = 0;

	else if ( (firstBit==0) && (secondBit==1) )
		rawValue = 1;

	else if ( (firstBit==1) && (secondBit==1) )
		rawValue = 2;

	else if ( (firstBit==1) && (secondBit==0) )
		rawValue = 3;

	double scaledValue = Scale(rawValue,0.0,3.0,-1.0,+1.0);

	return( scaledValue );
}

void NEURAL_NETWORK::Mutate(double mutationProbability) {

/*
	for (int i=0;i<numNodes;i++) {

		for (int j=0;j<numNodes;j++) {

			if ( Rand(0,1) < mutationProbability )

				Mutate(i,j);
		}
	}
*/

	int mutationOccurred = Mutate_Biased(mutationProbability);

	while ( !mutationOccurred )

		mutationOccurred = Mutate_Biased(mutationProbability);
}

void NEURAL_NETWORK::Print(void) {

	if ( weights )

		weights->Print();

	if ( nodeValues )

		nodeValues->Print();
}

void NEURAL_NETWORK::Save(ofstream *outFile) {

	if ( weights ) {
		(*outFile) << "1\n";
		weights->Write(outFile);
	}
	else
		(*outFile) << "0\n";

        (*outFile) << numSensors  << "\n";

        (*outFile) << numMotors  << "\n";
}

void NEURAL_NETWORK::Sensor_Set(int sensorIndex, double sensorValue) {

	// One bit
/*
	if ( sensorValue < 0 )
		sensorValue = -1;
	else
		sensorValue = +1;
	sensorValues->Set(0,sensorIndex,sensorValue);
*/

/*
	// Two bits
	int sensorBit1;
	int sensorBit2;
	     if ( sensorValue < -0.5 ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
	}
	else if ( sensorValue < +0.0 ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
	}
	else if ( sensorValue < +0.5 ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
	}
	else {
		sensorBit1 = +1;
		sensorBit2 = -1;
	}
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+0,sensorBit1);
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+1,sensorBit2);
*/

/*
	// Three bits
	int sensorBit1;
	int sensorBit2;
	int sensorBit3;
	     if ( sensorValue < -0.75 ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
		sensorBit3 = -1;
	}
	else if ( sensorValue < -0.5 ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
		sensorBit3 = +1;
	}
	else if ( sensorValue < -0.25 ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
		sensorBit3 = +1;
	}
	else if ( sensorValue < +0.0 ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
		sensorBit3 = -1;
	}
	else if ( sensorValue < +0.25 ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
		sensorBit3 = -1;
	}
	else if ( sensorValue < +0.5 ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
		sensorBit3 = +1;
	}
	else if ( sensorValue < +0.75 ) {
		sensorBit1 = +1;
		sensorBit2 = -1;
		sensorBit3 = +1;
	}
	else {
		sensorBit1 = +1;
		sensorBit2 = -1;
		sensorBit3 = -1;
	}
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+0,sensorBit1);
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+1,sensorBit2);
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+2,sensorBit3);
*/

	// Three bits
	int sensorBit1;
	int sensorBit2;
	int sensorBit3;
	int sensorBit4;
	     if ( sensorValue < -(7.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
		sensorBit3 = -1;
		sensorBit4 = -1;
	}
	else if ( sensorValue < -(6.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
		sensorBit3 = -1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < -(5.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
		sensorBit3 = +1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < -(4.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = -1;
		sensorBit3 = +1;
		sensorBit4 = -1;
	}
	else if ( sensorValue < -(3.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
		sensorBit3 = +1;
		sensorBit4 = -1;
	}
	else if ( sensorValue < -(2.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
		sensorBit3 = +1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < -(1.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
		sensorBit3 = -1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < -(0.0/8.0) ) {
		sensorBit1 = -1;
		sensorBit2 = +1;
		sensorBit3 = -1;
		sensorBit4 = -1;
	}

	else if ( sensorValue < +(1.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
		sensorBit3 = -1;
		sensorBit4 = -1;
	}
	else if ( sensorValue < +(2.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
		sensorBit3 = -1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < +(3.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
		sensorBit3 = +1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < +(4.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = +1;
		sensorBit3 = +1;
		sensorBit4 = -1;
	}
	else if ( sensorValue < +(5.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = -1;
		sensorBit3 = +1;
		sensorBit4 = -1;
	}
	else if ( sensorValue < +(6.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = -1;
		sensorBit3 = +1;
		sensorBit4 = +1;
	}
	else if ( sensorValue < +(7.0/8.0) ) {
		sensorBit1 = +1;
		sensorBit2 = -1;
		sensorBit3 = -1;
		sensorBit4 = +1;
	}
	else {
		sensorBit1 = +1;
		sensorBit2 = -1;
		sensorBit3 = -1;
		sensorBit4 = -1;
	}

	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+0,sensorBit1);
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+1,sensorBit2);
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+2,sensorBit3);
	sensorValues->Set(0,NODES_PER_SENSOR*sensorIndex+3,sensorBit4);
}

void NEURAL_NETWORK::Update(int timeStep) {

	if ( !edgeList )
		EdgeList_Create();

	// Initialize
	for (int j=0;	j<numNodes;	j++)

		temp->vals[j] = 0;

	// Add influences between nodes
	for (int i=0;	i<numConnections;	i++) {
 
		int sourceNode	= int(edgeList->vals[i*edgeList->width + 0]);
		int targetNode	= int(edgeList->vals[i*edgeList->width + 1]);
		double weight	=     edgeList->vals[i*edgeList->width + 2];

		// Source node is a sensor...
		if ( sourceNode < (NODES_PER_SENSOR*numSensors) ) {

			temp->vals[targetNode] = temp->vals[targetNode] + 

						(sensorValues->vals[sourceNode]*weight);
		}
		// Source node is a hidden or motor neuron...
		else {
			temp->vals[targetNode] = temp->vals[targetNode] + 

						(nodeValues->vals[sourceNode-(NODES_PER_SENSOR*numSensors)]*weight);
		}

	}


	for (int j=0;	j<numNodes;	j++) {

		if ( temp->vals[j]      < 0 )

			temp->vals[j]   = -1;

		else
			temp->vals[j]   = +1;
	}

	for (int j=0;	j<numNodes;	j++) {

		// Update the two motor and two hidden nodes,
		// but don't change the two sensor nodes.

		nodeValues->vals[j] = temp->vals[j];
	}
}

// Private methods ---------------------------------------------

void   NEURAL_NETWORK::Connection_Add(int nodeIndex) {

	int i = RandInt(0,((NODES_PER_SENSOR*numSensors)+numNodes)-1);

	while ( weights->Get(i,nodeIndex) != 0 )

		i = RandInt(0,((NODES_PER_SENSOR*numSensors)+numNodes)-1);

	if ( FlipCoin() )

		weights->Set(i,nodeIndex,-1);
	else
		weights->Set(i,nodeIndex,+1);
}

void   NEURAL_NETWORK::Connection_Remove(int nodeIndex) {

	int i = RandInt(0,((NODES_PER_SENSOR*numSensors)+numNodes)-1);

	while ( weights->Get(i,nodeIndex) == 0 )

		i = RandInt(0,((NODES_PER_SENSOR*numSensors)+numNodes)-1);

	weights->Set(i,nodeIndex,0);
}

void NEURAL_NETWORK::EdgeList_Create(void) {

	for (int i=0;i<weights->length;i++)

		for (int j=0;j<weights->width;j++)

			if ( weights->Get(i,j) != 0.0 )

				numConnections++;

	edgeList = new MATRIX(numConnections,3,0.0);

	numConnections = 0;

	for (int i=0;i<weights->length;i++)

		for (int j=0;j<weights->width;j++)

			if ( weights->Get(i,j) != 0.0 ) {

				edgeList->Set(numConnections,0,i);
				edgeList->Set(numConnections,1,j);
				edgeList->Set(numConnections,2,weights->Get(i,j));
				numConnections++;
			}
}

int NEURAL_NETWORK::FlipCoin(void) {

        return( Rand(0.0,1.0) > 0.5 );
}

double NEURAL_NETWORK::Incoming_Connections(int nodeIndex) {

	double incomingConnections = 0.0;

	for (int i=0;	i<((NODES_PER_SENSOR*numSensors)+numNodes);	i++)

		if ( weights->Get(i,nodeIndex) != 0 )

			incomingConnections++;

	return( incomingConnections );
}

void NEURAL_NETWORK::Initialize(void) {

	numNodes	= numMotors * NODES_PER_MOTOR;

	nodeValues 	= new MATRIX(1,numNodes,0.0);

	temp 		= new MATRIX(1,numNodes,0.0);

	inAFixedAttractor = false;

	numConnections	= 0;
	edgeList	= NULL;

	sensorValues	= new MATRIX(1,(NODES_PER_SENSOR*numSensors),0.0);
}

void NEURAL_NETWORK::Mutate(int i, int j) {

	int originalValue = int(weights->Get(i,j));

	switch ( originalValue ) {

	case -1:
		if ( FlipCoin() )
			weights->Set(i,j,0);
		else
			weights->Set(i,j,+1);
		break;

	case 0:
		if ( FlipCoin() )
			weights->Set(i,j,-1);
		else
			weights->Set(i,j,+1);
		break;

	case +1:
		if ( FlipCoin() )
			weights->Set(i,j,-1);
		else
			weights->Set(i,j,0);
		break;
	}
}

int NEURAL_NETWORK::Mutate_Biased(double mutationProbability) {

	int mutationOccurred = false;

	for (int j=0;	j<numNodes;	j++) {

		if ( Rand(0,1) < mutationProbability ) {

			Connection_Remove(j);
			Connection_Add(j);
			mutationOccurred = true;

/*
			double incomingConnections = Incoming_Connections(j);

			double probabilityOfRemovingAConnection =

				(40*incomingConnections) /
 
				( (40*incomingConnections) + (numNodes-incomingConnections) );

			if ( Rand(0,1) < probabilityOfRemovingAConnection )

				Connection_Remove(j);

			else
				Connection_Add(j);
*/

			mutationOccurred = true;
		}
	}

	return( mutationOccurred );
}

double NEURAL_NETWORK::Rand(double min, double max) {

        double zeroToOne = ((double)rand()) / RAND_MAX;
        double returnVal;

        returnVal = (zeroToOne * (max-min)) + min;
        return returnVal;
}

int NEURAL_NETWORK::RandInt(int min, int max) {

        if ( min == max )
                return( min );
        else
                return( (rand() % (max-min+1)) + min );
}

double NEURAL_NETWORK::Scale(double value, double min1, double max1,
                                           double min2, double max2) {

        if ( min1 < 0 )
                value = value - min1;
        else
                value = value + min1;

        return( (value*(max2-min2)/(max1-min1)) + min2 );

}

void NEURAL_NETWORK::Weights_Initialize_NumIncomingConnections(int nic) {

	for (int j=0;	j<numNodes;	j++)

		for (int k=0;	k<nic;	k++)

			Connection_Add(j);
}

#endif

