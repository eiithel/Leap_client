/*
 * Client.cpp
 *
 *  Created on: 9 avr. 2016
 *      Author: ethel
 */

#include "Client.h"


namespace tools {

// Sensor Patterns
const char* CHANNEL_SWIPE 	= "/gesture/type_swipe";
const char* CHANNEL_CIRCLE 	= "/gesture/type_circle";

static bool mIsNewCircle = false;


}  // namespace tools




Client::Client(): _host("localhost"), _port(1883), _keepalive(60), _clean_session(true),_mosq(NULL),circleDetected(false){

}

Client::~Client() {

	mosquitto_destroy(_mosq);
	mosquitto_lib_cleanup();

}


bool Client::is_new_circle()
{
	circleDetected = tools::mIsNewCircle;
	tools::mIsNewCircle = false;
	//le fait de rajouter cette ligne a permis l'écriture sur le port série... etrange.
	std::cout << "valeur de circleDetected: " << circleDetected << std::endl;
	return circleDetected;

}

bool Client::send_message(const  char * _message, const char *topic)
{
	int ret = mosquitto_publish(_mosq, NULL, topic, strlen(_message), _message,1,false);
	return ( ret == MOSQ_ERR_SUCCESS );
}




/*Cette fonction sera appelée dès
 *qu'un message sera envoyé sur le channel intéressant
 *stocker les messages dans des structures.
 */
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message
		*message)
{
	if(message->payloadlen){
		printf("fonction message_callback est appelee\n");
		printf("%s %s\n", message->topic, message->payload);

		if( strcmp( message->topic, tools::CHANNEL_CIRCLE ) == 0 ){
			printf("cercle detecte!!!!\n");
			tools::mIsNewCircle = true;
			/*if(strcmp((const char*)(message->payload),"1")==0){
				tools::mIsNewCircle = true;
			}*/
			//Client::circleDetected = true;

		}

	}else{
		printf("%s (null)\n", message->topic);
	}


	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, "/tmp/#", 2);
		//send_message(mosq,"envoie depuis le client");//ok
		//Subscribe to type_circle information.
		mosquitto_subscribe(mosq, NULL, tools::CHANNEL_CIRCLE, 2);
		mosquitto_subscribe(mosq, NULL, tools::CHANNEL_SWIPE, 2);

	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const
		int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	printf("%s\n", str);
}

int Client::init_connection(){

	mosquitto_lib_init();
	_mosq = mosquitto_new(_id, _clean_session, NULL);
	if(!_mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}
	mosquitto_log_callback_set(_mosq, my_log_callback);

	mosquitto_connect_callback_set(_mosq, my_connect_callback);
	mosquitto_message_callback_set(_mosq, my_message_callback);
	mosquitto_subscribe_callback_set(_mosq, my_subscribe_callback);

	if(mosquitto_connect(_mosq, _host, _port, _keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}

}

int Client::subscribe(const char *sub){
	//mosquitto_subscribe(mosq, NULL, sub, 2);
	int ret= mosquitto_subscribe(_mosq, NULL, sub, 2);
	return(ret == MOSQ_ERR_SUCCESS);

}






