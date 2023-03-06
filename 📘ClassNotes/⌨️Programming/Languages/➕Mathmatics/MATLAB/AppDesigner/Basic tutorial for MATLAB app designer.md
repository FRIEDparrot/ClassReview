## public functions 

delete App and Exit: 
```erlang 
function delete(app)
            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
```

note that `tab` is really useful in MATLAB


## How to transfer data  between different  APPs
firstly we define a `DialogApp` as another windows to be called in the current App

note that we should define two apps (mlapp) 

```erlang
properties (Access = private)
	DialogApp                   % Dialog box app
	CurrentSize = 20;           % Surface sample size
	CurrentColormap = 'Parula'; % Colormap
end
```

Use public method to communicate between different apps

```erlang
methods (Access = public)
	function updateplot(app, sz, c)
		% Store inputs as properties
		app.CurrentSize = sz;
		app.CurrentColormap = c;
		
		% Update plot 
		Z = peaks(sz);
		surf(app.UIAxes,Z);
		colormap(app.UIAxes,c);
		
		% Re-enable the Plot Options button
		app.OptionsButton.Enable = 'on';
	end
	function MainAppCloseRequest(app, event)
            % Delete both apps
            delete(app.DialogApp)
            delete(app)
	end
end 
```


in the main APP, we can use the following code to call the dialog app like : 
```erlang
function OptionsButtonPushed(app, event)
	% Disable Plot Options button while dialog is open
	app.OptionsButton.Enable = 'off';
	
	% Open the options dialog and pass inputs
	app.DialogApp = DialogAppExample(app, app.CurrentSize, app.CurrentColormap);
end
```
in the code above we just transfer the parameters in the functions 

then we define the properties `callingAPP` in the DialogApp as: 
<mark style="background: transparent; color: yellow">Note that startup is a callback </mark>
```erlang

methods (Access = private)
	% Code that executes after component creation
	function StartupFcn(app, mainapp, sz, c)
		% Store main app in property for CloseRequestFcn to use
		app.CallingApp = mainapp;
		% Update UI with input values
		app.EditField.Value = sz;
		app.DropDown.Value = c;
	end
```

in the *dialog Function*, call the public method of the MainAPP directly 
```erlang
function ButtonPushed(app, event)
            % Call main app's public function
            updateplot(app.CallingApp, app.EditField.Value, app.DropDown.Value);
            % Delete the dialog box
            delete(app)
end
```



