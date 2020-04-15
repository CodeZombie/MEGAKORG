namespace Resources {

String index_html = "<html> <head> <style>table, td{border-style: solid; border-width: 1px; border-color: black;}</style> <script>function selectBank(){var e=prompt('Enter a value (0-22):','4');if(null!=e){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==this.readyState&&(console.log('Request returned with code '+this.status),document.getElementById('bankKnob').innerText='EDIT SELECT ('+this.response+')')},t.open('GET','/selectBank?value='+e,!0),t.send()}}function turnKnob(e){var t=prompt('Enter a value (0-255):','64');if(null!=t){var n=new XMLHttpRequest;n.onreadystatechange=function(){4==this.readyState&&(console.log('Request returned with code '+this.status),document.getElementById('knobButton_'+e).innerText='KNOB '+e+'('+this.response+')')},n.open('GET','/turnKnob?knob='+e+'&value='+t,!0),n.send()}}</script> </head> <body> <table> <tbody> <tr> <td></td><td><a id='knobButton_0' href='javascript:turnKnob(0);'>KNOB 0 (0)</a></td><td><a id='knobButton_1' href='javascript:turnKnob(1);'>KNOB 1 (0)</a></td><td><a id='knobButton_2' href='javascript:turnKnob(2);'>KNOB 2 (0)</a></td><td><a id='knobButton_3' href='javascript:turnKnob(3);'>KNOB 3 (0)</a></td><td><a id='knobButton_4' href='javascript:turnKnob(4);'>KNOB 4 (0)</a></td></tr><tr> <td><a id='bankKnob' href='javascript:selectBank();'>EDIT SELECT (0)</a></td></tr></tbody> </table> </body></html>";

};
/*
String index_html = "<html> \
    <head> \
        <style> \
            table { \
                border: 1px solid #000; \
            } \
        </style> \
        <script> \
            function changeParam(parameter){ \  
                var value = prompt('Enter a value (0-255):', '64'); \
                if(value == null) { return; } \
                var xhttp = new XMLHttpRequest(); \
                xhttp.onreadystatechange = function() { \
                    if (this.readyState == 4) { \
                        console.log('Request returned with code ' + this.status) \
                    } \
                }; \
                xhttp.open('GET', '/changeParam?parameter=' + parameter + '&value=' + value, true); \
                xhttp.send(); \
            } \
        </script>  \
    </head> \
    <body> \
        <table> \
            <tr> \
                <th>Bank</th> \
                <th>Knob 0</th> \
                <th>Knob 1</th> \
                <th>Knob 2</th> \
                <th>Knob 3</th> \
                <th>Knob 4</th> \
              </tr> \
              <tr> \
                <td>Voice</td> \
                <td><a href='javascript:changeParam(0);'>Synth/Vocoder</a></td> \
                <td><a href='javascript:changeParam(1);'>Single/Layer</a></td> \
                <td><a href='javascript:changeParam(2);'>Voice Assign</a></td> \
                <td><a href='javascript:changeParam(3);'>Trigger Mode</a></td> \
                <td><a href='javascript:changeParam(4);'>Unison Detune</a></td> \
              </tr> \
              <tr> \
                <td>Pitch</td> \
                <td><a href='javascript:changeParam(5)'>Transpose</a></td> \
                <td><a href='javascript:changeParam(6);'>Tune</a></td> \
                <td><a href='javascript:changeParam(7);'>Portamento</a></td> \
                <td><a href='javascript:changeParam(8);'>Bend Range</a></td> \
                <td><a href='javascript:changeParam(9);'>Vibrato Int</a></td> \
              </tr> \
              <tr> \
                <td>Osc 1</td> \
                <td><a href='javascript:changeParam(10);'>Wave</a></td> \
                <td><a href='javascript:changeParam(11);'>Control 1</a></td> \
                <td><a href='javascript:changeParam(12);'>Control 2</a></td> \
              </tr> \
              <tr> \
                <td>Delay</td> \
                <td><a href='javascript:changeParam(13);'>Type</a></td> \
                <td><a href='javascript:changeParam(14);'>Tempo Sync</a></td> \
                <td><a href='javascript:changeParam(15);'>Delay Time</a></td> \
                <td><a href='javascript:changeParam(16);'>Delay Depth</a></td> \
              </tr> \
        </table> \
    </body> \
</html>";

}
*/
