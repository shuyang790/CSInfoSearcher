var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index',
    {
      title: 'CS Faculty / University Searcher',
      readme: 'Input a university name or faculty name, and magic happens!'
      //alertContent: 'This is an alert'
    });
});

router.get('/about', function(req, res, next) {
  res.render('about',
    {
      title: 'CS Faculty / University Searcher',
      content: 'This is our demo for Computer Network course project.'
    });
});

router.get('/contact', function(req, res, next) {
  res.render('contact',
    {
      title: 'CS Faculty / University Searcher',
      content: 'Below is our contact information:',
      infos: {
          'Shuyang Shi': 'shuyang790@gmail.com',
          'Xingyu Lin': 'sean.linxingyu@pku.edu.cn',
          'Dayou Du': 'dudayou1901@gmail.com',
          'Yize Xie': 'xyzoffice@163.com'
      }
    });
});

module.exports = router;
