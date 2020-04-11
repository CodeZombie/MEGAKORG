namespace Resources {

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
