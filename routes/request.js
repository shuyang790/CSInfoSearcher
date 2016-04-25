var express = require('express');
var spawn = require('child_process').spawn;
var router = express.Router();

router.post('/', function(req, res) {

  keyword = req.body.keyword;
  console.log("body.keyword: " + keyword);
  queryCgi = spawn('cgi-bin/query', [keyword]);
  queryCgi.stdout.on('data', function (data) {
      console.log('standard output:\n' + data);
      queryResult = data;
      res.render('index',
        {
          title: 'CS Faculty / University Searcher',
          alertContent: 'Last search: ' + new Date(),
          queryResult: JSON.parse(queryResult)
        });
  });


});

module.exports = router;
