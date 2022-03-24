import './App.css';


function App() {
  return (
    <div className="App" >

      <h1 id="Title">Advanced Solar Data Collector</h1>
      <h2 id="Sub-Title">Brighter Tomorow</h2>

      <div className="Rectangle">

        <iframe id="PowerWeek" className="Dash-Board" src="http://localhost:3000/d-solo/9X9S_DEnz/basic-graphics?orgId=1&from=now-7d&to=now&refresh=30s&panelId=12" ></iframe>
        <iframe className="Dash-Board" src="http://localhost:3000/d-solo/9X9S_DEnz/basic-graphics?orgId=1&from=now-24h&to=now&refresh=30s&panelId=12" ></iframe>
        <iframe className="Dash-Board" src="http://localhost:3000/d-solo/9X9S_DEnz/basic-graphics?orgId=1&from=now-24h&to=now&refresh=30s&panelId=10"></iframe>
        <iframe className="Dash-Board" src="http://localhost:3000/d-solo/9X9S_DEnz/basic-graphics?orgId=1&from=now-24h&to=now&refresh=30s&panelId=6"></iframe>
        <iframe className="Dash-Board" src="http://localhost:3000/d-solo/9X9S_DEnz/basic-graphics?orgId=1&from=now-24h&to=now&refresh=30s&panelId=14"></iframe>
 
      </div>
    </div>
  );
}

export default App;
