/*** JavaScript.cpp: */
#include "Cap_WebServer.h"

void cap_Webserver::handleScriptJS() {
  String js = "";
  js += "function showTab(id) {\n";
  js += "  document.querySelectorAll('.tabContent').forEach(div => div.classList.remove('active'));\n";
  js += "  document.getElementById(id).classList.add('active');\n";
  js += "  if (id === 'infos') loadDoc('/info', 'infos');\n";
  js += "  if (id === 'config') loadSchedulerConfig();\n";
  js += "}\n";
  /***********************************/
  js += "function loadTrackOptions() {\n";
  js += "  fetch('/count')\n";
  js += "    .then(res => res.text())\n";
  js += "    .then(txt => {\n";
  js += "      let match = txt.match(/\\d+/);\n";
  js += "      let total = match ? parseInt(match[0]) : 0;\n";
  js += "      let sel = document.getElementById('trackSelect');\n";
  js += "      sel.innerHTML = '';\n";
  js += "      for (let i = 1; i <= total; i++) {\n";
  js += "        let opt = document.createElement('option');\n";
  js += "        opt.value = i;\n";
  js += "        opt.text = 'Track ' + i;\n";
  js += "        sel.appendChild(opt);\n";
  js += "      }\n";
  js += "    });\n";
  js += "}\n";

  js += "function playSelected() {\n";
  js += "  let track = document.getElementById('trackSelect').value;\n";
  js += "  fetch('/play?track=' + track);\n";
  js += "}\n";

  js += "function playRandom() {\n";
  js += "  fetch('/random');\n";
  js += "}\n";

  js += "function stopPlayback() {\n";
  js += "  fetch('/stop');\n";
  js += "}\n";

  /***********************************/
  js += "function toggleOutput(pin) {\n";
  js += "  fetch('/setoutput?pin=' + pin + '&state=1')\n";
  js += "    .then(() => {\n";
  js += "      setTimeout(() => {\n";
  js += "        fetch('/setoutput?pin=' + pin + '&state=0');\n";
  js += "      }, 30000);\n";
  js += "    });\n";
  js += "}\n";

  js += "function updateOutputs() {\n";
  js += "  fetch('/getoutputs')\n";
  js += "    .then(res => res.json())\n";
  js += "    .then(data => {\n";
  js += "      for (let i = 0; i < 4; i++) {\n";
  js += "        document.getElementById('stateP' + i).innerText = data['P' + i] ? 'ON' : 'OFF';\n";
  js += "      }\n";
  js += "    });\n";
  js += "}\n";

  js += "setInterval(updateOutputs, 2000);\n";
  js += "window.onload = updateOutputs;\n";

  /***********************************/
  js += "function loadCSS() {\n";
  js += "  fetch('/thema.css')\n";
  js += "    .then(res => res.text())\n";
  js += "    .then(txt => {\n";
  js += "      document.getElementById('cssArea').value = txt;\n";
  js += "    });\n";
  js += "}\n";

  js += "function saveCSS() {\n";
  js += "  const css = document.getElementById('cssArea').value;\n";
  js += "  fetch('/savecss', {\n";
  js += "    method: 'POST',\n";
  js += "    headers: { 'Content-Type': 'text/plain' },\n";
  js += "    body: css\n";
  js += "  })\n";
  js += "  .then(res => res.text())\n";
  js += "  .then(msg => alert('Gespeichert: ' + msg));\n";
  js += "}\n";

  js += "function reloadCSS() {\n";
  js += "  loadCSS();\n";
  js += "}\n";
  /***********************************/
  js += "function loadDoc(url, id) {\n";
  js += "  var xhttp = new XMLHttpRequest();\n";
  js += "  xhttp.onreadystatechange = function() {\n";
  js += "    if (this.readyState == 4 && this.status == 200) {\n";
  js += "      document.getElementById(id).innerHTML = this.responseText;\n";
  js += "    }\n";
  js += "  };\n";
  js += "  xhttp.open(\"GET\", url, true);\n";
  js += "  xhttp.send();\n";
  js += "}\n";
  /***********************************/
  js += "setInterval(function() {\n";
  js += "  loadDoc(\"/iplocal\", \"iplocal\");\n";
  js += "  loadDoc(\"/ssid\", \"ssid\");\n";
  js += "  loadDoc(\"/count\", \"count\");\n";
  js += "  loadFilesystemStats();\n";
  js += "}, 10000);\n";

  /***********************************/
  js += "setInterval(function() {\n";
  js += "  loadDoc(\"/mp3status\", \"mp3status\");\n";
  js += "  loadDoc(\"/datum\", \"datum\");\n";
  js += "  loadDoc(\"/uhrzeit\", \"uhrzeit\");\n";
  js += "}, 1000);\n";
  /***********************************/
  js += "function loadSchedulerConfig() {\n";
  js += "  fetch(\"/schedulerconfig\")\n";
  js += "    .then(res => res.json())\n";
  js += "    .then(cfg => {\n";
  js += "      const dayOrder = [1, 2, 3, 4, 5, 6, 0];\n";
  js += "      const dayNames = [\"So\", \"Mo\", \"Di\", \"Mi\", \"Do\", \"Fr\", \"Sa\"];\n";
  js += "      let html = `<form id='configForm'>`;\n";
  js += "      html += `<label><input type='checkbox' id='enabled' ${cfg.enabled ? \"checked\" : \"\"}> Scheduler aktiv</label><br><br>`;\n";
  js += "      html += `<fieldset><legend>Aktive Wochentage:</legend>`;\n";
  js += "      dayOrder.forEach(i => {\n";
  js += "      html += `<label style='margin-right:10px;'><input type='checkbox' class='day' data-index='${i}' ${cfg.activeDays[i] ? \"checked\" : \"\"}> ${dayNames[i]}</label>`;";
  js += "      });\n";
  js += "      html += `</fieldset><br>`;\n";
  js += "      html += `<label>Intervall (Minuten): <input type='number' id='interval' value='${cfg.intervalMinutes || 60}' min='1' max='1440'></label><br><br>`;\n";
  js += "      html += `<button type='button' class='btn' onclick='saveConfig()'>Speichern</button>`;\n";
  js += "      html += `</form>`;\n";
  js += "      document.getElementById(\"json_config\").innerHTML = html;\n";
  js += "    });\n";
  js += "}\n";

  /***********************************/

  /***********************************/
  js += "function updateVolume(vol) {\n";
  js += "  fetch(\"/setvolume?value=\" + vol)\n";
  js += "    .then(res => res.text())\n";
  js += "    .then(txt => console.log(\"Setvolume Antwort:\", txt))\n";
  js += "    .catch(err => console.error(\"Setvolume Fehler:\", err));\n";
  js += "}\n";
  /***********************************/
  js += "function fetchVolume() {\n";
  js += "  fetch(\"/volume\")\n";
  js += "    .then(res => res.text())\n";
  js += "    .then(vol => {\n";
  js += "      document.getElementById(\"volumeSlider\").value = vol;\n";
  js += "    });\n";
  js += "}\n";
  /***********************************/
  js += "setInterval(fetchVolume, 3000);\n";
  js += "window.onload = fetchVolume;\n";

  /***********************************/
  js += "function uploadFavicon() {\n";
  js += "  const fileInput = document.getElementById('faviconUpload');\n";
  js += "  if (!fileInput.files.length) return alert('Keine Datei ausgewählt');\n";
  js += "  const formData = new FormData();\n";
  js += "  formData.append('upload', fileInput.files[0]);\n";
  js += "  fetch('/uploadfavicon', {\n";
  js += "    method: 'POST',\n";
  js += "    body: formData\n";
  js += "  })\n";
  js += "  .then(res => res.text())\n";
  js += "  .then(msg => alert('Upload abgeschlossen: ' + msg));\n";
  js += "}\n";
  /***********************************/
  js += "function saveConfig() {\n";
  js += "  const enabled = document.getElementById(\"enabled\").checked;\n";
  js += "  const interval = parseInt(document.getElementById(\"interval\").value);\n";
  js += "  const activeDays = Array(7).fill(false);\n";
  js += "  document.querySelectorAll(\".day\").forEach(cb => {\n";
  js += "    const i = parseInt(cb.dataset.index);\n";
  js += "    activeDays[i] = cb.checked;\n";
  js += "  });\n";
  js += "  const config = {\n";
  js += "    enabled: enabled,\n";
  js += "    intervalMinutes: interval,\n";
  js += "    activeDays: activeDays\n";
  js += "  };\n";
  js += "  fetch(\"/saveconfig\", {\n";
  js += "    method: \"POST\",\n";
  js += "    headers: { \"Content-Type\": \"application/json\" },\n";
  js += "    body: JSON.stringify(config)\n";
  js += "  })\n";
  js += "  .then(res => res.text())\n";
  js += "  .then(msg => alert(\"Gespeichert: \" + msg));\n";
  js += "}\n";
  /***********************************/
  js += "function loadFilesystemStats() {\n";
  js += "  fetch('/fsinfo')\n";
  js += "    .then(res => res.json())\n";
  js += "    .then(data => {\n";
  js += "      document.getElementById('filecount').innerText = data.count + ' Dateien';\n";
  js += "      document.getElementById('freespace').innerText = data.free + ' KB';\n";
  js += "    });\n";
  js += "}\n";
  /***********************************/

  /***********************************/
  js += "window.onload = function() {\n";
  js += "  loadDoc(\"/info\", \"infos\");\n";
  js += "  loadSchedulerConfig();\n";
  js += "  setTimeout(loadTrackOptions, 3000);\n";
  js += "  loadCSS();\n";
  js += "  showTab('infos');\n";  // Standard-Tab
  js += "};\n";
  /***********************************/
  server.send(200, "application/javascript", js);
}
