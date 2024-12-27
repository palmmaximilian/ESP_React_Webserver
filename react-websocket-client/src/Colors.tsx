import { HuePicker, ColorResult } from "react-color";
import { useWordclockData } from "./AppContext/AppContext.tsx";
import { Container, Row, Col, Form } from "react-bootstrap";

export function Colors() {
  const { brightness, setBrightness, color, setColor, updateBrightness, updateColor } = useWordclockData();

  return (
    <Container fluid="sm">
      <Row className="mb-3">
        <Col>
          <h2>Color Settings</h2>
        </Col>
      </Row>

      <Row className="align-items-center mb-3">
        <Col xs={12} md={4}>
          <Form.Label>Brightness</Form.Label>
        </Col>
        <Col xs={2} md={2}>
          {`${brightness}%`}
        </Col>
        <Col xs={10} md={6}>
          <Form.Range
            value={brightness}
            max={100}
            min={0}
            step={5}
            onChange={(e) => {
              setBrightness(e.target.value);
            }}
            onMouseUp={(e) => {
              const newBrightness = (e.target as HTMLInputElement).value;
              updateBrightness(newBrightness);
            }}
            onTouchEnd={(e) => {
              const newBrightness = (e.target as HTMLInputElement).value;
              updateBrightness(newBrightness);
            }}
          />
        </Col>
      </Row>

      <Row className="align-items-center mb-3">
        <Col xs={12} md={4}>
          <Form.Label>Selected Color</Form.Label>
        </Col>
        <Col xs={2} md={2}>
          <div
            style={{
              width: "50px",
              height: "50px",
              backgroundColor: typeof color === 'string' ? color : color.hex,
              border: "1px solid #000",
              borderRadius: "4px",
            }}
          />
        </Col>
        <Col xs={10} md={6}>
          <HuePicker
            color={color}
            onChange={(color: ColorResult) => setColor(color.hex)}
            onChangeComplete={(color: ColorResult) => updateColor(color.hex)}
            width="100%"
          />
        </Col>
      </Row>
    </Container>
  );
}
