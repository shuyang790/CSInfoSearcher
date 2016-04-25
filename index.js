var express = require('express');
var app = express();

app.set('view engine', 'jade');

// app.get('/', function (req, res) {
// 	res.render('index', {message: "Hello World!"})
// //  res.send('Hello World!');
// });

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});
