var express = require('express');
var router = express.Router();

router.post('/', function(req, res) {
  res.render('index',
    {
      title: 'CS Faculty / University Searcher',
      alertContent: 'Last search: ' + new Date()
    });
});

module.exports = router;
