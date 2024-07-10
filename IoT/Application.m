%% Channels IDs & Channels API & Talkback
% Sensor ChannelID
sensorChannelID = 2472648;
% Sensor Channel ReadAPI
sensorReadAPI = 'HRV40X4D515PGVFT';

% Plant ChannelID
plantChannelID = 2500177;
% Plant Channel ReadAPI
plantReadAPI = 'TLGVH539FJS33WKW';

%system modeChannelID
systemChannelID = 2522218;
%system mode ChannelID ReadAPI
systemReadAPI = '28MU4DB4XNB9IEWX';

%Farm Status
farmChannelID =  2582460;
farmReadAPI = '0AQF15G9DUT63LB8';
farmWriteAPI = 'N259XWULP4VWNWNE';


% TalkBack app ID
TalkBack_ID = '52134';
% TalkBack app API key
TalkBack_apikey = 'XJB7KX6ZNU6CDFD4';
%url to send commands to TalkbackAPP
url =  strcat('https://api.thingspeak.com/talkbacks/',TalkBack_ID,'/commands.json');

% ThingSpeak Alerts API Key
alertApiKey = 'TAKKc2NXdxJhbsnnNcF';%
% Set the address for the HTTP call
alertURL = "https://api.thingspeak.com/alerts/send";
% webwrite uses weboptions to add required headers. Alerts need a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey]);


%% Functions
%%Function To Check Moisture
function pump = CheckMoisture (soilMoisture,plantMoisture,RainSensor,Tank,lastPumpCommand,TalkBack_apikey,url)

% Check Conditions for Moisture and handle potential webwrite errors
if ~isnan(soilMoisture) && ~ isnan(plantMoisture)
    try
        % Handle the soil that needs irrigation and no rain
        if (soilMoisture >= plantMoisture && RainSensor &&~Tank)  % rain --> 0
            if (lastPumpCommand ~= 1)
                pump = 1;
                webwrite(url, 'api_key', TalkBack_apikey, 'command_string', 'TURN_ON_PUMP');
                disp("TURN_ON_PUMP");
            else
                 pump = 1;
            end
        else
            if (lastPumpCommand ~= 0)
                pump = 0;
                webwrite(url, 'api_key', TalkBack_apikey, 'command_string', 'TURN_OFF_PUMP');
                disp("True OFF PUMP");
            else
                pump = 0;                
            end
        end
    catch ME
        % Handle errors in the webwrite function
        disp('An error occurred:');
        disp(ME.message);
    end
end
end

%%Function To Check Raining
function roof = CheckRain(RainSensor,Moisture,plantMoisture,lastRoofCommand,TalkBack_apikey,url)

if ~isnan(RainSensor) && ~isnan(plantMoisture)
    if isempty(lastRoofCommand)
        lastRoofCommand = -1; % Initialize to empty if it's the first call
    end
    % Check if it is raining
    if RainSensor == 0 % RainSensor = 0 --> Raining
        % Check if the soil needs irrigation
        try
            if Moisture >= plantMoisture
                if (lastRoofCommand ~= 1)
                    roof = 1;
                    webwrite(url, 'api_key', TalkBack_apikey, 'command_string', 'OPEN_ROOF');
                    disp("TURN_ON_ROOF");
                else
                    roof = 1;
                end
            else
                if (lastRoofCommand ~= 0)
                     roof = 0;
                    webwrite(url, 'api_key', TalkBack_apikey, 'command_string', 'CLOSE_ROOF');
                    disp("TURN_OFF_ROOF");
                else
                    roof = 0;
                end
            end
        catch ME
            % Handle errors in the webwrite function
            disp('An error occurred:');
            disp(ME.message);
        end
    else
        disp("It is not raining, no action taken.");
    end
end
end

%%Function To Check Temperature & Humidity
function roof = CheckTemperature(Temperature,Humidity,plantTemperature,plantHumidity,lastRoofCommand,TalkBack_apikey,url)
% Check temperature and humidity conditions
if ~(isnan(Temperature) || isnan(Humidity)) && ~(isnan(plantTemperature) || isnan(plantHumidity))
    if isempty(lastRoofCommand)
        lastRoofCommand = -1; % Initialize to empty if it's the first call
    end
    try
        if Temperature >= plantTemperature
            if (lastRoofCommand ~= 0)
                roof = 0;
                webwrite(url, 'api_key', TalkBack_apikey, 'command_string', 'OPEN_ROOF');
                disp("TURN_OFF_ROOF");
            else
                roof = 0;
            end
        elseif Temperature <= plantTemperature
            if (lastRoofCommand ~= 1 )
                roof = 1;
                webwrite(url, 'api_key', TalkBack_apikey, 'command_string', 'CLOSE_ROOF');
                disp("TURN_ON_ROOF");
            else
                roof = 1;
            end
        end       
    catch ME
        % Handle errors in the webwrite function
        disp('An error occurred:');
        disp(ME.message);
    end
end
end

%%Function To  Check Tank Level
function MonitorTankLevel(TankData,alertURL,options)
% Tank full --> 0
if ~isnan(TankData)
    if TankData == 1
        alertBody = 'Tank Is Empty';
        alertSubject = 'Tank Status Alert: Empty';
        disp('Tank is empty');
        % Send alert
        try
            response = webwrite(alertURL, "body", alertBody, "subject", alertSubject, options);
            disp('Alert sent successfully.');
            disp("Response:");
            disp(response);
        catch ME
            % Handle errors in the webwrite function
            disp('Failed to send alert:');
            disp(ME.message);
        end
    elseif TankData == 0 % Full --> 0
        disp('Tank is not empty.');
    end
end
end

%%Function To  CheckFlameSensor
function MonitorFlameSensor(FlameData,alertURL,options)
if isnan(FlameData)
    disp(["Flame Sensors is ",FlameData])
else
    % Check if fire is detected
    if FlameData == 1 % fire detected
        % Define the alert message
        alert_body = 'There is a fire';
        alert_subject = 'ThingSpeak Alert email';

        disp('Fire detected! Sending alert...');
        try
            % Send the alert
            response = webwrite(alertURL, "body", alert_body, "subject", alert_subject, options);
            disp('Alert sent successfully.');
            disp("Response:");
            disp(response);
        catch ME
            % Handle errors in the webwrite function
            disp('An error occurred while sending the alert:');
            disp(ME.message);
        end
    else
        disp('No fire detected.');
    end

end
end

%% Check System Auto or Manual
% System mode
systemMode = thingSpeakRead(systemChannelID, 'Fields', 1, ReadKey=systemReadAPI);
% Auto = 1 , manual = 0
if systemMode == 1 || isnan(systemMode)
%% Reading Sensors Data
    try
        SensorsData = thingSpeakRead(sensorChannelID, ReadKey=sensorReadAPI);
        %Moisture SensorData
        soilMoisture = SensorsData(1);
        % Temperature
        Temperature = SensorsData(2);
        % Humidity
        Humidity = SensorsData(3);
        %FlameData
        FlameData = SensorsData(4);
        %RainSensor SensorData
        RainSensor = SensorsData(5);
        %TankData
        TankData=SensorsData(6);
    catch ME
        % Handle errors in the Read function
        disp('An error occurred while geting sensors data:');
        disp(ME.message);
    end
%% Reading Plant Data
    try       
        plantData= thingSpeakRead(plantChannelID,ReadKey=plantReadAPI);
        if (~isnumeric(plantData))
            disp("Data is non-numric")
        end
        plantMoisture = plantData(1);
        plantTemperature = plantData(2);
        plantHumidity = plantData(3);
    catch ME
        % Handle errors in the Read function
        disp('An error occurred while geting plant data:');
        disp(ME.message);
    end
%% Reading lastcommand
 try       
        FarmStatus = thingSpeakRead(farmChannelID,ReadKey=farmReadAPI);
        if isempty(FarmStatus)
            lastPumpCommand = -1;
            lastRoofCommand = -1;
        else
            lastPumpCommand = FarmStatus(1);
            lastRoofCommand = FarmStatus(2);
        end
      
catch ME
        % Handle errors in the Read function
        disp('An error occurred while geting Last Command:');
        disp(ME.message);
end
    %% Call Functions
    if ~isempty(SensorsData) %handle if an error occurred while geting Data

        %Check Moisture and control pump system

        pump = CheckMoisture(soilMoisture,plantMoisture,RainSensor,TankData,lastPumpCommand,TalkBack_apikey,url);

        %Check Raining and Check Temperature&Humidity
        % control roof system
        if  RainSensor == 0 % RainSensor = 0 --> Raining
           roof = CheckRain(RainSensor,soilMoisture,plantMoisture,lastRoofCommand,TalkBack_apikey,url);
        else
            roof = CheckTemperature(Temperature,Humidity,plantTemperature,plantHumidity,lastRoofCommand,TalkBack_apikey,url);
        end
        
        %MonitorTankLevel and send alert if Tank empty
        MonitorTankLevel(TankData,alertURL,options)

        % Monitor FlameSensor and send alert if find Fire
        MonitorFlameSensor(FlameData,alertURL,options)

        % Write to Farm Status
         if pump ~= lastPumpCommand && roof ~= lastRoofCommand
            thingSpeakWrite(farmChannelID,[pump,roof],'WriteKey',farmWriteAPI) ;
         elseif pump ~= lastPumpCommand
            thingSpeakWrite(farmChannelID,[pump,roof],'WriteKey',farmWriteAPI) ;
         elseif  roof ~= lastRoofCommand
             thingSpeakWrite(farmChannelID,[pump,roof],'WriteKey',farmWriteAPI) ;
        end
    end
end
