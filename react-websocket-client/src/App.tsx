import { Route, Routes } from "react-router-dom";
import "./App.css";
// import Home from "./Home";
import { Connection } from "./Connection";
import { Settings } from "./Settings";

function App() {
  return (
    <div className="App">
      <Routes>
        <Route path="/" element={<Connection />} />
        <Route path="/wifi" element={<Connection />} />
        <Route path="/settings" element={<Settings/>} />
        {/* <Route path="/settings" element={<Settings />} /> */}
      </Routes>
    </div>
  );
}

export default App;
