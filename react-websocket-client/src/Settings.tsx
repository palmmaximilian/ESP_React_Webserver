import { SetStateAction, useContext, useEffect, useState } from "react";
import { WordclockDataContext } from "./AppContext/AppContext.tsx";
import Papa from "papaparse";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Form from "react-bootstrap/Form";
import Dropdown from "react-bootstrap/Dropdown";
import { ButtonGroup } from "react-bootstrap";

export function Settings() {
  const WordclockData = useContext(WordclockDataContext);
  if (!WordclockData) {
    throw new Error("WordclockDataContext is not available");
  }

  const {
    timezone,
    setTimezone,
    setTimezoneCode,
    nightmode,
    setNightmode,
    nightmodebrightness,
    setNightmodebright,
    nightmodeFrom,
    setNightmodeFrom,
    nightmodeTo,
    setNightmodeTo,
    language,
    setLanguage,
  } = WordclockData;

  const LanguageList = [
    { label: "German", value: "0" },
    { label: "English", value: "1" },
  ];

  const [zones, setZones] = useState<[string, string][]>([]);
  const [searchQuery, setSearchQuery] = useState("");

  const filteredZones = zones.filter((zone) =>
    zone[0].toLowerCase().includes(searchQuery.toLowerCase())
  );

  useEffect(() => {}, [timezone]);

  useEffect(() => {
    fetch("zones.csv")
      .then((response) => response.text())
      .then((data) => {
        Papa.parse(data, {
          complete: (result: { data: SetStateAction<never[]> }) => {
            setZones(result.data as [string, string][]);
          },
        });
      });
  }, []);

  return (
    <Container fluid="sm">
      <Row className="my-3">
        <Col>
          <h2>Time</h2>
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <Form.Label>Timezone</Form.Label>
        </Col>
        <Col xs={8}>
          <Dropdown className="w-100 text-end" as={ButtonGroup}>
            <Dropdown.Toggle
              variant="success"
              id="dropdown-basic"
              className="w-100"
            >
              {timezone}
            </Dropdown.Toggle>

            <Dropdown.Menu
              className="w-100"
              style={{
                maxHeight: "200px",
                overflowY: "auto",
                overflowX: "hidden",
                paddingTop: "0px",
              }}
            >
              <div
                style={{
                  position: "sticky",
                  top: 0,
                  zIndex: 1,
                  backgroundColor: "white",
                }}
              >
                <Form.Control
                  type="text"
                  placeholder="Search..."
                  value={searchQuery}
                  onChange={(e) => {
                    setSearchQuery(e.target.value);
                  }}
                  className="mb-2"
                />
              </div>

              {filteredZones.map((zone) => (
                <Dropdown.Item
                  key={zone[0]}
                  onClick={() => {
                    setTimezone(zone[0]);
                    setTimezoneCode(zone[1]);
                    WordclockData.updateTimezone(zone[0]);
                    WordclockData.updateTimezoneCode(zone[1]);
                  }}
                >
                  {zone[0]}
                </Dropdown.Item>
              ))}
            </Dropdown.Menu>
          </Dropdown>
        </Col>
      </Row>

      <hr className="divider" />

      <Row className="my-3">
        <Col>
          <h2>Nightmode</h2>
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <Form.Label>From</Form.Label>
        </Col>
        <Col xs={8}>
          <Form.Control
            type="time"
            value={nightmodeFrom}
            onChange={(e) => {
              setNightmodeFrom(e.target.value);
              WordclockData.updateNightmodeFrom(e.target.value);
              console.log("Setting nightmode from: " + e.target.value);
            }}
          />
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <Form.Label>To</Form.Label>
        </Col>
        <Col xs={8}>
          <Form.Control
            type="time"
            value={nightmodeTo}
            onChange={(e) => {
              setNightmodeTo(e.target.value);
              WordclockData.updateNightmodeTo(e.target.value);
              console.log("Setting nightmode to: " + e.target.value);
            }}
          />
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <Form.Label>Activate?</Form.Label>
        </Col>
        <Col xs={8}>
          <Form.Check
            type="checkbox"
            checked={nightmode}
            onChange={(e) => {
              setNightmode(e.target.checked);
              WordclockData.updateNightmode(e.target.checked);
            }}
          />
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4} md={4}>
          <Form.Label>Brightness</Form.Label>
        </Col>

        <Col xs={8} md={8}>
          <p>{nightmodebrightness}%</p>
        </Col>
      </Row>
      <Row className="my-3">
        <Col xs={0} md={4}></Col>
        <Col xs={12} md={8}>
          <Form.Control
            type="range"
            min="0"
            max="100"
            step="5"
            value={nightmodebrightness}
            onChange={(e) => {
              setNightmodebright(e.target.value);
              WordclockData.updateNightmodebrightness(e.target.value);
            }}
          />
        </Col>
      </Row>

      <hr className="divider" />

      <Row className="my-3">
        <Col>
          <h2>Language</h2>
        </Col>
      </Row>

      <Row className="my-3">
        <Col xs={4}>
          <Form.Label>Language</Form.Label>
        </Col>
        <Col xs={8}>
          <Form.Control
            as="select"
            value={language}
            onChange={(e) => {
              setLanguage(e.target.value);
              WordclockData.updateLanguage(e.target.value);
            }}
          >
            {LanguageList.map((lang) => (
              <option key={lang.value} value={lang.value}>
                {lang.label}
              </option>
            ))}
          </Form.Control>
        </Col>
      </Row>
    </Container>
  );
}

export default Settings;
