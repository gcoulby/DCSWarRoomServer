
# DCS WarRoom Server

## Overview
DCS WarRoom Server is a backend C++ server designed to parse and transmit gameplay data from DCS World to a frontend application via a REST API. It efficiently handles real-time game data to facilitate dynamic map updates and unit tracking.

## Key Features
- Real-time parsing of DCS World gameplay data.
- REST API endpoints to provide game entity data.
- Utilizes modern C++ standards and libraries like Boost and Crow for efficient network communication.

## Installation

### Prerequisites
- C++17
- Boost libraries
- Crow framework

### Building the Server
1. Clone the repository:
2. Open the sln file in Visual Studio and build the project.
3. Set the Additional Include Directories for your Boost libraries in the project settings.

### Running the Server
1. Configure your server settings in `settings.conf`:
   ```plaintext
   DCS_HOST=localhost
   DCS_PORT=8080
   DCS_PASSWORD=your_password
   ```

2. Execute the server:
   Run the server either in the Visual Studio debugger or by executing the built binary.

## API Usage
The server provides multiple endpoints to interact with the game data:
- `GET /api/v1/entities`: Fetches basic entity data.
- `GET /api/v1/entities?mode=simple`: Fetches unit entity data in simple form
- `GET /api/v1/entities?mode=full`: Fetches unit entity data in full form
- `GET /api/v1/entities/<id>`: Provides detailed entity data including all properties.


## Contributing
Contributions to DCS WarRoom Server are welcome. Please fork the repository, make your changes, and submit a pull request for review.

## License
This project is licensed under the MIT License - see the `LICENSE` file for details.

## Acknowledgements
This server is designed to interface with DCS World by Eagle Dynamics but is not officially endorsed or affiliated with them.
