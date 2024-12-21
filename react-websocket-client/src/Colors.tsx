import { useContext } from "react";
import { HuePicker } from "react-color";
import {
  useWordclockData,
  WordclockDataContext,
} from "./AppContext/AppContext.tsx";
import "./App.css"; // Assuming you have a CSS file for styles

export function Colors() {
  // const [sliderState, setSliderState] = useState(50);
  // const [color, setColor] = useState('#ffffff');
  const WordclockData = useContext(WordclockDataContext);
  if (!WordclockData) {
    throw new Error("WordclockDataContext is not available");
  }
  const { brightness, setBrightness } = useWordclockData();
  // const [localColor, setLocalColor] = useState('#ffffff');
  const { color, setColor } = useWordclockData();

  return (
    <div>
      <div className="rowView">
        <h2 className="titleText">Color</h2>
      </div>

      <div className="rowView">
        <span className="baseText" style={{ width: 150 }}>
          Brightness
        </span>
        <span className="baseText">Value: {brightness}%</span>
      </div>

      <input
        type="range"
        value={brightness}
        max={100}
        min={0}
        step={5}
        onChange={(e) => {
          setBrightness(e.target.value);
          WordclockData.updateBrightness(e.target.value);
        }}
      />
      <HuePicker
        color={color}
        onChangeComplete={(color) => {
          setColor(color.hex);
          WordclockData.updateColor(color.hex.toString());
        }}
      />
      <div className="rowView">
        <span className="baseText">Selected Color</span>
        <div
          style={{
            width: 50,
            height: 50,
            backgroundColor: color.toString(),
            border: "1px solid #000",
            marginLeft: 10,
          }}
        />
      </div>
    </div>
  );
}
