import React from 'react';

import AppBar from 'material-ui/AppBar';
import Badge from 'material-ui/Badge';
import FontIcon from 'material-ui/FontIcon';

const styles = {
	fontIcon: {
		color: 'white',
	},
	AppIcon: {
		marginTop: 11,
		color: 'white',
	},
};

class AppHeader extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			usersOnline: 0
		};
	}

	userConnected(usersOnline) {
		this.setState({
			usersOnline: usersOnline
		});
	}

	userDisconnected(usersOnline) {
		this.setState({
			usersOnline: usersOnline
		});
	}

	componentWillMount() {
		socket.on('user-connected', (usersOnline) => {
			this.userConnected(usersOnline);
		});
		socket.on('user-disconnected', (usersOnline) => {
			this.userDisconnected(usersOnline);
		});
	}

	render() {
		// setTimeout(this.forceUpdate(), 5000);
		return (
			<div>
				<AppBar 
					title="WSN"
					iconElementLeft={<FontIcon className="fa fa-microchip" style={styles.AppIcon} />}
					iconElementRight={<Badge badgeContent={this.state.usersOnline} secondary={true}><FontIcon className="fa fa-user" style={styles.fontIcon} /></Badge>}
				/>
			</div>
		);
	}
}

export default AppHeader;