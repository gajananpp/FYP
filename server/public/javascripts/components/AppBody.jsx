import React from 'react';

import { Card, CardHeader, CardMedia, CardActions, CardTitle, CardText } from 'material-ui/Card';
import Toggle from 'material-ui/Toggle';
import Slider from 'material-ui/Slider';
import Divider from 'material-ui/Divider';
import Dialog from 'material-ui/Dialog';
import TextField from 'material-ui/TextField';
import RaisedButton from 'material-ui/RaisedButton';

import RTChart from 'react-rt-chart';

const key = "13et1055";

const chart = {
	color: {
		pattern: ['red']
	}
};

class AppBody extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			expandedHumidity: false,
			expandedTemparature: false,
			expandedBrightness: false,
			expandedForce: false,

			humidity: 0,
			temperature: 0,
			brightness: 0,
			force: 0,

			loadLightValue: 0,
			loadMotorValue: 0,

			manualControlToggled: false,
			authenticationDialog: false,

			keyTextField : "",
		};
	}

	// handleToggle = (event, isChecked) => {
	// 	if (isChecked)
	// 		socket.emit('turn-on-loadLight');
	// 	else
	// 		socket.emit('turn-off-loadLight');
	// };

	handleSliderChange = (event, newValue) => {
		// socket.emit('loadLight-value-change', `loadLight:${newValue}`);
		return;
	};

	updateReadings(readings) {
		console.log(readings);
		this.setState({
			humidity: readings[0].value,
			temperature: readings[1].value,
			brightness: readings[2].value,
			force: readings[3].value

			// loadLightValue: readings.loadLightValue,
		});
	}

	toggleManualLoadControl = (event, isInputChecked) => {
		let validClient = this.verifyValidity();
		socket.emit("toggle-manual-control", validClient ? 1 : 0);
	};

	handleKeyInputChange = (event, newValue) => {
		this.setState({
			keyTextField: newValue
		});
	};

	verifyValidity = () => {
		if (this.state.keyTextField === key) {
			this.setState({
				authenticationDialog: false,
				manualControlToggled: !this.state.manualControlToggled,
				keyTextField: ""
			});
			return true;
		} else {
			this.setState({
				authenticationDialog: true,
				manualControlToggled: this.state.manualControlToggled,
				keyTextField: ""
			});
			return false;
		}
	};

	closeDialog = () => {
		this.setState({
			authenticationDialog: false
		});
	};

	componentWillMount() {
		socket.on('readings', (readings) => {
			// console.log(readings);
			this.updateReadings(readings);
		});
		this.forceUpdate();
	}

	render() {
		const dialogActions = [
			<RaisedButton label="Ok" primary={true} onClick={this.verifyValidity} style={{marginRight: 30}} />,
			<RaisedButton label="Cancel" primary={true} onClick={this.closeDialog} />
		];

		const dataArray = [
			{
				sensor: "DHT11",
				quantity: "Humidity",
				unit: "%",
				currentReading: this.state.humidity,
				chart: {color: {pattern: ['#1f77b4']}},
				expanded: this.state.expandedHumidity,
				expand: (expanded) => this.setState({expandedHumidity: !this.state.expandedHumidity}),
			},
			{
				sensor: "DHT11",
				quantity: "Temperature",
				unit: "℃",
				currentReading: this.state.temperature,
				chart: {color: {pattern: ['#ff7f0e']}},
				expanded: this.state.expandedTemparature,
				expand: (expanded) => this.setState({expandedTemparature: !this.state.expandedTemparature}),
			},
			{
				sensor: "LDR",
				quantity: "Brightness",
				unit: "%",
				currentReading: this.state.brightness,
				load: "LED Bulb",
				loadValue: this.state.loadLightValue,
				chart: {color: {pattern: ['#2ca02c']}},
				expanded: this.state.expandedBrightness,
				expand: (expanded) => this.setState({expandedBrightness: !this.state.expandedBrightness}),
			}, {
				sensor: "FSR",
				quantity: "Force",
				unit: "N",
				currentReading: this.state.force,
				load: "Conveyer Belt",
				loadValue: this.state.loadMotorValue,
				chart: {color: {pattern: ['#d62728']}},
				expanded: this.state.expandedForce,
				expand: (expanded) => this.setState({expandedForce: !this.state.expandedForce}),
			}
		];
		return (
			<div>
				<div style={{maxWidth: 250, marginTop: 20, marginLeft: '2%'}}>
					<Toggle
						toggled={this.state.manualControlToggled} 
						label="Manual Load Control"
						onToggle={this.toggleManualLoadControl}
					/>
				</div>

				<div style={{margin: 'auto', marginTop: 20, marginBottom: 20}} >
					{dataArray.map((obj, index) => (
						<Card style={{width: '96%', margin: 'auto', marginTop: 8}} key={index} expanded={obj.expanded} onExpandChange={() => obj.expand(obj.expanded)}>
							<CardHeader
								title={obj.sensor}
								subtitle={obj.quantity}
								actAsExpander={true}
								showExpandableButton={true}
							 />
							 <Divider />
				 			<CardMedia
				 				expandable={true}
				 				overlay={<CardTitle title={`Current ${obj.quantity}: ${obj.currentReading} ${obj.unit}`} subtitle={""} />}
				 			>
				 				<RTChart
				 					chart={obj.chart}
				 					style={{marginTop: 10}}
				 					flow={{duration: 5000}}
				 					maxValues={8} 
				 					fields={[obj.quantity]}
				 					data={
				 						{
				 							date: new Date(),
				 							[obj.quantity]: obj.currentReading,
				 						}
				 					}
				 				/>
				 			</CardMedia>
				 			<Divider />
				 			{ obj.load ?
				 			(<CardActions expandable={true} style={{height: 50}}>
				 				{/*<Toggle 
				 					label={`Toggle ${obj.load}`}
				 					defaultToggled={obj.loadStatus ? true : false}
				 					onToggle={ this.handleToggle }
				 				/>*/}
				 				<Slider 
				 					min={0}
				 					max={255}
				 					step={1}
				 					value={obj.loadValue}
				 					onChange={this.handleSliderChange}
				 				/>
				 			</CardActions>)
				 			: null }
						</Card>
					))}
				</div>

				<Dialog
					title="Enter Key"
					modal={true}
					open={this.state.authenticationDialog}
					actions={dialogActions}
				>
					<TextField 
						hintText="Enter key to control loads"
						type="password"
						onChange={this.handleKeyInputChange}
					/>
				</Dialog>
			</div>

		);
	}
}

export default AppBody;



