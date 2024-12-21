import { useContext } from "react";
import {
  useWordclockData,
  WordclockDataContext,
} from "./AppContext/AppContext.tsx"; // Import the WordclockDataContext

import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Dropdown from "react-bootstrap/Dropdown";

export function Connection() {
  const WordclockData = useContext(WordclockDataContext);
  if (!WordclockData) {
    throw new Error("WordclockDataContext is not available");
  }
  const { ssidList } = useWordclockData();
  const { ssid, setSsid } = useWordclockData();
  const { password, setPassword } = useWordclockData();
  const { connectionStatus } = useWordclockData();

  return (
    <Container fluid="sm">
      <Row className="my-3">
        <Col xs={6}>Current Status:</Col>
        <Col xs={6}>{connectionStatus}</Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <label>Available Networks:</label>
        </Col>
        <Col xs={8}>
          <Dropdown className="w-100">
            <Dropdown.Toggle
              variant="success"
              id="dropdown-basic"
              className="w-100"
            >
              {ssid || "Select SSID"}
            </Dropdown.Toggle>
            <Dropdown.Menu className="w-100">
              {ssidList.map((item) => (
          <Dropdown.Item
            key={item}
            onClick={() => {
              setSsid(item);
            }}
          >
            {item}
          </Dropdown.Item>
              ))}
            </Dropdown.Menu>
          </Dropdown>

          {/* <input
          type="text"
          value={ssid}
          onChange={(e) => setSsid(e.target.value)}
          placeholder={ssid}
        /> */}
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <label>Password:</label>
        </Col>
        <Col xs={8}>
          <input
            type="password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            placeholder="Password"
            className="w-100"
          />
        </Col>
      </Row>

      <Row className="my-3 justify-content-center" >
        <Col>
          <button
            className="btn btn-primary w-100"
            onClick={() => {
              if (ssid !== "") {
                setSsid(ssid);
              }
              if (password !== "") {
                setPassword(password);
              }
              WordclockData.updateWifiCredentials();
            }}
          >
            Set Wifi Connection
          </button>
        </Col>
      </Row>
    </Container>
  );
}
