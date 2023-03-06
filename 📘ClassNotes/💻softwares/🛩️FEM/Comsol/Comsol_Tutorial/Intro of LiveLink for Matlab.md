
## Libraries and docs for reference
Type `help mli` and `help sli` for more information

use `tab` to get suggestions 

Type `mphapplicationlibraries` to open the Application Library

`````ad-note
title: mphdoc
collapse: close

mphdoc can show the documentation of one part 

for example, we can 
Use `mphdoc(model.param)` to Show the documentation for the model.param 
![[Pasted image 20230227220623.png|400]]
`````

also we can save the model as `.m` file in comsol to study the basic operations

The LiveLink  for MATLAB can help to 
1. preprocess data and include the models 
2. Extract data from the models 
3. Manipulate the models from the command line or script to parameterize the geometry, physics, and the solution scheme 
4. for customized apps developing, etc. 

## Basic Summary for Command
the Comsol API can be list as follows 
| Interface in Comsol | API in MATLAB                                                          |
| ------------------- | ---------------------------------------------------------------------- |
| Model               | `model = ModelUtil.create(Model), model.name('abc.mph')`               |
| parameter settings  | `model .param.set('L', '9{cm}') `                                      |
| component creating  | `model .create('comp1')`                                               |
| geometry            | `model.geom.create('geom1, 3')`                                        |
| physics             | `model.physics.create('ec','Conductive Media', 'geom1')`               |
|                     | `model.physics.create('ht','HeatTransfer', 'geom1')`                   |
| multiphysics        | `model.multiphysics.create('emh1','ElectromaneticHeatSource','geom1')` |
| mesh                | `model.mesh.create('mesh1')`                                           |
| study               | `model.study.create('std1')`                                           |
| stationary study    | `model.study('std1').feature.create('stat', 'Stationary')`             |
| solution            | `model.sol.create('sol1')`                                             |
| result              | `model.result.create('pg1','PlotGroup3D')`                                                                       |

we can use `mphopen` for <mark style="background: transparent; color: yellow">opening the model in the current folder</mark>
use `mphmesh(model)` for generating the mesh like
![[Pasted image 20230227180610.png|200]]

Also we can use `mphplot(model, 'pg2')` to plot the result of the corresponding picture
![[Pasted image 20230227184332.png|200]]

`mphnavigator` can show exactly all the element of the batch, which contains all the nodes and the methods, which we can copy directly from the list

![[Pasted image 20230227185244.png|400]]

to show geometry, we can use `mphgeom` command 
and `model.study('std1').run` can run the solution 

![[Pasted image 20230227190401.png|300]]
![[Pasted image 20230227191027.png|300]]
![[Pasted image 20230227191042.png|300]]
![[Pasted image 20230227191103.png|300]]
Use `mphsearch` for searching the relevant parameters 

If we want to use a Function defined by the MATLAB, we can use the following  method: 
![[Pasted image 20230227192256.png|650]]

![[Pasted image 20230227200137.png|400]]

![[Pasted image 20230227202352.png|400]]

view the Geometry on the work plane
![[Pasted image 20230227203210.png|250]]

## How to Change parameters 

`````ad-caution
title: How to Launch the COMSOL livelink with MATLAB
collapse: open
Use `mphlaunch` command to launch the model first

firstly we can use `mphopen Micro_resistance_beam.mph`

if many models are opened, use `mphlaunch(model)`

`````

Use `a = mphgetexpressions(model.param)` to list all the parameters of the model 

`mphevaluate` is for evaluating the parameters of the model, like: 
```erlang
mphevaluate(model,'r0')
ans =
    0.5000
```

also `mpheval` can also show the expression and the value of corresponding variable 

`mphinterp(model,'T', 'coord', [0 -0.01 1e-3 ; 0,-0.02 1e-3])`
this is the interpolation from the first point to the second point of temperature

`model.param.set('r0', '1000[mm]')` can set the relevant model for comsol 

`mphmax (model, 'T', 'Volume')`  

`mphdataset(model, 'dset1')`

## First Step to Build a model 

`````ad-note
title: Firstly
collapse: open
Use
```erlang
import com.comsol.model.*
import com.comsol.model.util.*
```

at the beginning of the file is necessay
`````

If we model by creating a model object on the COMSOL server, we can use
`model = ModelUtil.create('Model')`

`````ad-note
title: inititlize the component
collapse: open
example :  if we want to initialize a 3D object view in the axis, we can do it by: 
```erlang
app.model.component.create('comp1', true);
app.model.geom.create('geom1', 3); % initialize a 3D object
```
`````

`````ad-note
title: Axis Definition
collapse: open

for plotting the result in the User Defined axis, like developing an App
```erlang
axis(app.ImageAxes, 'image');
```

`````

## Result Generate 
If we want to get the matrix that we solved after calculation we can reference the `mphdoc(model)` for the corresponding matrix and how to check it. 
![[Pasted image 20230227221928.png|400]]

global evaluation `freq = mphglobal(model,'lambda')/(2*pi)`

to view the specific geometry, we can just use 
`mphgeom(model, <geomtag>, 'view', 'auto')`

`mphgeom(model, <geomtag>, 'entity', entity, 'selection', <idx>)`

## Parameter Definition 

`````ad-example
title: global parameters
collapse: open
`model.param.set('L', '9[cm]', 'length of the busbar')`

`model.param.set('Vtot','20[mV]', 'Applied electric potential')`

`````

