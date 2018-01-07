<?php

	function Connection(){
		$server="fdb18.awardspace.net";
		$user="xxxxx";
		$pass="xxxxx";
		$db="xxxxx";
	   	
		/*$connection = mysql_connect($server, $user, $pass);*/
                
                $connection = mysqli_connect($server, $user, $pass, $db);
                

		if (!$connection) {
	    	die('MySQL ERROR: ' . mysql_error());
		}
		
		/*mysql_select_db($db) or die( 'MySQL ERROR: '. mysql_error() );*/

		return $connection;
	}
?>
