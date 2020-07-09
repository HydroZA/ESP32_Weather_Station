<?php
  // including FusionCharts PHP wrapper
  include("fusioncharts-suite-xt/integrations/php/fusioncharts-wrapper/fusioncharts.php"); 
?>

<html>
<head>
  <!-- including FusionCharts JavaScript file -->
	<script type="text/javascript" src="https://cdn.fusioncharts.com/fusioncharts/latest/fusioncharts.js"></script>
	<script type="text/javascript" src="https://cdn.fusioncharts.com/fusioncharts/latest/themes/fusioncharts.theme.fusion.js"></script>
</head>
 
<body>
<?php
    //setting up connection with database
  $host_db="localhost"; // MySQL host server (might vary depending upon user)
  $user_db="esp_board"; // MySQL database username
  $pass_db="xxxx"; // MySQL password
  $name_db="esp_data"; // name of the database
 
 	$dbhandle = new mysqli($host_db, $user_db, $pass_db, $name_db);
 
	// if ($dbhandle -> connect_error)
	// {
 //    	exit("There was an error with your connection: ".$dbhandle - > connect_error);
	// }

	$sql = "SELECT temperature, humidity, reading_time FROM SensorData WHERE location=\"Outside\" ORDER BY id ASC";
 
	$result = $dbhandle->query($sql) or exit("Error code ({$dbhandle->errno}): {$dbhandle->error}");

	if ($result) {
  
    // creating an associative array to store the chart attributes        
    $arrDataTemp = array(
      "chart" => array(
        "theme" => "candy",
        "caption" => "Temperature Over Time",
        "subcaption" => "Outside - June 2020",
        "captionFontSize" => "24",
        "paletteColors" => "#A2A5FC, #41CBE3, #EEDA54, #BB423F #,F35685",
        "baseFont" => "Quicksand, sans-serif",
        //"labelStep" => "100",
        "showvalues" => "0"
        //more chart configuration options
      )
    );
    $arrDataHum = array(
      "chart" => array(
        "theme" => "candy",
        "caption" => "Humidity Over Time",
        "subcaption" => "Outside - June 2020",
        "captionFontSize" => "24",
        "paletteColors" => "#A2A5FC, #41CBE3, #EEDA54, #BB423F #,F35685",
        "baseFont" => "Quicksand, sans-serif",
        //"labelStep" => "100",
        "showvalues" => "0"
        //more chart configuration options
      )
    );
    $arrDataTemp["data"] = array();
    $arrDataHum["data"] = array();

    // iterating over each data and pushing it into $arrData array
    $i = 0; 
    while ($row = mysqli_fetch_array($result)) {
      if ($i < 3)
      {
        $i = $i + 1;
        continue;
      }
      $i = 0;
      array_push($arrDataTemp["data"], array(
        "label" => $row["reading_time"],
        "value" => $row["temperature"]
      ));
    }

    mysqli_data_seek($result, 0);
    
    // repeat for humidity data
    $i = 0; 
    while ($row = mysqli_fetch_array($result)) {
      if ($i < 3)
      {
        $i = $i + 1;
        continue;
      }
      $i = 0;
      
      array_push($arrDataHum["data"], array(
        "label" => $row["reading_time"],
        "value" => $row["humidity"]
      ));
    }
   
    $jsonEncodedDataTemp = json_encode($arrDataTemp);
    $jsonEncodedDataHum = json_encode($arrDataHum);
  }
  // creating FusionCharts instance
  $tempChart = new FusionCharts("line", "temperatureChart" , "100%", "450", "temp-chart", "json", $jsonEncodedDataTemp);
  $humChart = new FusionCharts("line", "humidityChart", "100%", "450", "hum-chart", "json", $jsonEncodedDataHum);
  
  // FusionCharts render method
  $tempChart->render();
  $humChart->render();

  // closing database connection      
  $dbhandle->close();
?>
<div id="temp-chart">Temperature Chart</div>
<div id="hum-chart">Humidity Chart</div>
<form action="php/esp-data.php">
  <input type="submit" value="View Raw Data"/>
</form>
</body>
</html>