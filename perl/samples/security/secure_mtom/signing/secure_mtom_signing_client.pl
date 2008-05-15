#!/usr/bin/env perl

use WSO2::WSF;

my $payload =<<E;
<ns1:upload xmlns:ns1="http://php.axis2.org/samples/mtom">
  <ns1:fileName>test.jpg</ns1:fileName>
  <ns1:image xmlmime:contentType="image/jpeg" xmlns:xmlmime="http://www.w3.org/2004/06/xmlmime">
    <xop:Include xmlns:xop="http://www.w3.org/2004/08/xop/include" href="cid:myid1"></xop:Include>
  </ns1:image>
</ns1:upload>
E

open FILE, "< ../../../mtom/axis2.jpg";
undef $/;
my $image = <FILE>;

my $msg = new WSO2::WSF::WSMessage(
    { 'to'          => 'http://localhost:8787/samples/security/secure_mtom/signing/service.php',
      'action'      => 'http://wso2.org/upload',
      'attachments' => { 'myid1' => $image },
      'payload'     => $payload
    } );

open MYC, "< ../../keys/alice_cert.cert";
undef $/;
my $mycert = <MYC>;

open MYK, "< ../../keys/alice_key.pem";
undef $/;
my $mykey = <MYK>;

open REC, "< ../../keys/bob_cert.cert";
undef $/;
my $reccert = <REC>;

open XML, "< policySignOnlyMtom.xml";
undef $/;
my $policy_xml = <XML>;

my $policy = new WSO2::WSF::WSPolicy( $policy_xml );

my $sec_token = new WSO2::WSF::WSSecurityToken( { 'privateKey'          => $mykey,
						  'certificate'         => $mycert,
						  'receiverCertificate' => $reccert
						} );

my $client = new WSO2::WSF::WSClient( { 'useMTOM'       => 'TRUE',
				        'useSOAP'       => '1.1',
				        'useWSA'        => 'TRUE',
				        'policy'        => $policy,
				        'securityToken' => $sec_token
				      } );

my $res_msg = $client->request( $msg );

print $res_msg->{str};
