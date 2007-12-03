<?php
/*
 * Copyright 2005,2006 WSO2, Inc. http://wso2.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

$reqPayloadString = <<<XML
	<ns1:echoString xmlns:ns1="http://php.axis2.org/samples">
		<text>Hello World!</text>
	</ns1:echoString>
XML;

try {

   $client = new WSClient(array( "to"=>"http://localhost/samples/echo_service.php"));

   $child = new WSHeader(array("ns"=>"http://test.org",
	                       "name"=>"header2",
			        "data"=>"value2"));

   $header = new WSHeader(array("ns"=>"http://test.org",
	                    "name"=>"header1",
				 "data"=>array($child)));

   $msg = new WSMessage($reqPayloadString ,
       array("inputHeaders" => array($header)));
   $client->request($msg);

   $sentMsg = $client->getLastRequest();
   $recvMsg = $client->getLastResponse();        

   echo "\nSent message \n";
   echo htmlspecialchars($sentMsg);
   
   echo "\n\n Received message \n";
   echo htmlspecialchars($recvMsg);



} catch (Exception $e) {
	if ($e instanceof WSFault) {
		printf("Soap Fault: %s\n", $e->Reason);
	} else {
		printf("Message = %s\n",$e->getMessage());
	}
}
?>
