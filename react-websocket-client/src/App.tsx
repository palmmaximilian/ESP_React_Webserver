import { Route, Routes } from "react-router-dom";
// import "./App.css";
// import Home from "./Home";
import { Connection } from "./Connection";
import { Settings } from "./Settings";
import { Colors } from "./Colors";

import 'bootstrap/dist/css/bootstrap.min.css';

function App() {
  return (
    <div className="App">
      <Routes>
        <Route path="/" element={<Connection />} />
        <Route path="/wifi" element={<Connection />} />
        <Route path="/settings" element={<Settings/>} />
        <Route path="/colors" element={<Colors/>} />
        {/* <Route path="/settings" element={<Settings />} /> */}
      </Routes>
    </div>
  );
}

export default App;
