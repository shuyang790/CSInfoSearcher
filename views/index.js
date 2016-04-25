var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index',
    {
      title: 'CS Faculty / University Searcher',
      //alertContent: 'This is an alert'
    });
});

module.exports = router;
