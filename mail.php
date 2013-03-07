#!/usr/bin/php-cgi
<?php print('<?xml version="1.0" encoding="utf-8"?>'); print("\n"); ?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<title>
		</title>
		<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
		<meta name="viewport" content="initial-scale=1.0, target-densitydpi=device-dpi" />
		<!-- this is for mobile (Android) Chrome -->
		<meta name="viewport" content="initial-scale=1.0, width=device-height">
		<!--  mobile Safari, FireFox, Opera Mobile  -->
		<link rel="stylesheet" href="https://ajax.aspnetcdn.com/ajax/jquery.mobile/1.1.1/jquery.mobile-1.1.1.min.css" />
		<link rel="stylesheet" href="my.css" />
		<link href='http://fonts.googleapis.com/css?family=Titillium+Web:400,700,200' rel='stylesheet' type='text/css'>
		<script src="libs/modernizr.js"></script>
		<style type="text/css">
			/* App custom styles */
			#mapPhoto {
			-moz-border-radius: 10px;
			-webkit-border-radius: 10px;
			border-radius: 10px; /* future proofing */
			-khtml-border-radius: 10px; /* for old Konqueror browsers */
			}
			#twitterPic {
			padding: 0px 10px;
			}
			.ui-page {
			background: transparent;
			font-family: 'Titillium Web', sans-serif;
			}
			.ui-content{
			background: transparent;
			}
			#signatureparent {
			color:darkblue;
			background-color:darkgrey;
			/*max-width:600px;*/
			padding:10px;
			}
			/*This is the div within which the signature canvas is fitted*/
			#signature {
			border: 2px dotted black;
			background-color:white;
			height:7em
			}
			/* Drawing the 'gripper' for touch-enabled devices */ 
			html.touch #content {
			float:left;
			width:92%;
			}
			html.touch #scrollgrabber {
			float:right;
			width:4%;
			margin-right:2%;
			background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAAFCAAAAACh79lDAAAAAXNSR0IArs4c6QAAABJJREFUCB1jmMmQxjCT4T/DfwAPLgOXlrt3IwAAAABJRU5ErkJggg==)
			}
			html.borderradius #scrollgrabber {
			border-radius: 1em;
			}
		</style>
		</style>
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"></script>
		<script src="https://ajax.aspnetcdn.com/ajax/jquery.mobile/1.1.1/jquery.mobile-1.1.1.min.js"></script>
		<script src="my.js"></script>
	</head>
	<body>
		<!-- Home -->
		<div data-role="page" id="page1" data-theme="a" >
			<div data-role="content">
				<div style="">
					<center>
						<img style="width: 196px height: 91px" src="MEB-logo.png" />
					</center>
				</div>
				<?php
					require_once('signature-to-image.php');
					require_once('phpMail/class.phpmailer.php');
					$json = $_POST['output']; // From Signature Pad
					$img = sigJsonToImage($json);
					
					// Save to file
					imagepng($img, 'signatureSIG.png');
					
					// Destroy the image in memory when complete
					imagedestroy($img);
					
					
					$checkList = "";
					if(isset($_POST['name'])) {
					$checkList = implode(",", $_POST['check_list']);
					$email = $_POST['email']; // required
					$name = $_POST['name']; // required
					$phone = $_POST['phone']; // required
					$zip = $_POST['zip']; // required
					$addr = $_POST['addr']; // required
					$state = $_POST['state']; // required
					$city = $_POST['city']; // required
					$grade = $_POST['grade']; // required
					
					
					  $email_exp = '/^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$/';
					if(!preg_match($email_exp,$email)) {
					  echo "The Email Address you entered does not appear to be valid.<br />";
					}
					else{ 
					 $email_message = "Form details below.\n\n";
					function clean_string($string) {
					    $bad = array("content-type","bcc:","to:","cc:","href");
					    return str_replace($bad,"",$string);
					  }
					  $email_message .= "Email: ".clean_string($email)."\n";
					  $email_message .= "Name: ".clean_string($name)."\n";
					  $email_message .= "Phone: ".clean_string($phone)."\n";
					  $email_message .= "Zip: ".clean_string($zip)."\n";
					  $email_message .= "Address: ".clean_string($addr)."\n";
					  $email_message .= "State: ".clean_string($state)."\n";
					  $email_message .= "City: ".clean_string($city)."\n";
					  $email_message .= "Grade: ".clean_string($grade)."\n";  
					  $email_message .= "Position: ".clean_string($checkList)."\n";  
					   
					try {
					$mail = new PHPMailer(true); // the true param means it will throw exceptions on     errors, which we need to catch
					
					$mail->IsSMTP(); // telling the class to use SMTP
					$mail->Host       = "smtp.morningside.edu"; // SMTP server
					$mail->Port       = 25;                    // set the SMTP port
					$mail->SetFrom($email, $name);
					$mail->AddAddress('sdr001@morningside.edu', 'Sean R');
					$mail->Subject = 'Registration Form';
					
					$mail->AddEmbeddedImage("signatureSIG.png", "my-attach", "signatureSIG.png");
					$mail->Body = $email_message;
					
					$mail->Send();
					} catch (phpmailerException $e) {
					echo $e->errorMessage(); //Pretty error messages from PHPMailer
					} catch (Exception $e) {
					echo $e->getMessage(); //Boring error messages from anything else!
					}
					
					echo "Thank you for contacting us, we will get back to you soon.<br />";
					}
					}
					?>
				<ul data-role="listview" data-divider-theme="d" data-inset="true">
					<li data-role="list-divider" role="heading">
						Home
					</li>
					<li data-theme="c">
						<a href="#about" data-transition="pop">
						About
						</a>
					</li>
					<li data-theme="c">
						<a href="#camps" data-transition="pop">
						Satellite Camps
						</a>
					</li>
					<li data-theme="c">
						<a href="#contact" data-transition="pop">
						Contact
						</a>
					</li>
					<li data-theme="c">
						<a href="#register" data-transition="pop">
						Register
						</a>
					</li>
				</ul>
			</div>
			<div id="socialImages">
				<center>
				<a href="http://www.facebook.com/pages/Midwest-Elite-Basketball/6804530431?fref=ts">
				<img src="facebook_dark.png" /></a>
				<a href="https://twitter.com/MidwestEliteBB">
				<img id="twitterPic" src="twitter_dark.png" />
				</a>
				</ceneter>
			</div>
			<div data-theme="a" data-role="footer" data-position="fixed">
				<h3>
					www.midwestelitebasketball.com
				</h3>
			</div>
		</div>
	</body>
</html>