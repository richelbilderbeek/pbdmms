#include "pbd_qtmaindialog.h"

#include <cassert>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>
#include <QFileDialog>
#include "pbd.h"
#include "pbd_l_table_row.h"
#include "ui_pbd_qtmaindialog.h"

const int row_birth_good{0};
const int row_birth_incipient{1};
const int row_completion{2};
const int row_death_good{3};
const int row_death_incipient{4};
const int row_time{5};
const int row_seed{6};

pbd::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::pbd_qtmaindialog),
  m_nltt_plot_igtree_extinct{new QwtPlot(QwtText("nLTT (igtree extinct)"), this)},
  m_nltt_plot_recontree{new QwtPlot(QwtText("nLTT (recontree)"), this)},
  m_nltt_plot_igtree_extinct_line{new QwtPlotCurve("nLTT (igtree extinct)")},
  m_nltt_plot_recontree_line{new QwtPlotCurve("nLTT (recontree)")}

{
  ui->setupUi(this);

  //Add widgets
  assert(ui->scroll_area_contents->layout());
  ui->scroll_area_contents->layout()->addWidget(m_nltt_plot_recontree);
  ui->scroll_area_contents->layout()->addWidget(m_nltt_plot_igtree_extinct);

  m_nltt_plot_recontree->setMinimumHeight(400);
  m_nltt_plot_recontree->setAxisScale(QwtPlot::yLeft  , 0.0, 1.0, 0.1);
  m_nltt_plot_recontree->setAxisScale(QwtPlot::xBottom, 0.0, 1.0, 0.1);
  m_nltt_plot_recontree_line->attach(m_nltt_plot_recontree);
  m_nltt_plot_recontree_line->setStyle(QwtPlotCurve::Steps);
  m_nltt_plot_recontree_line->setPen(Qt::black, 2.0);

  m_nltt_plot_igtree_extinct->setMinimumHeight(400);
  m_nltt_plot_igtree_extinct->setAxisScale(QwtPlot::yLeft  , 0.0, 1.0, 0.1);
  m_nltt_plot_igtree_extinct->setAxisScale(QwtPlot::xBottom, 0.0, 1.0, 0.1);
  m_nltt_plot_igtree_extinct_line->attach(m_nltt_plot_igtree_extinct);
  m_nltt_plot_igtree_extinct_line->setStyle(QwtPlotCurve::Steps);
  m_nltt_plot_igtree_extinct_line->setPen(Qt::black, 2.0);
  set_parameters(create_parameters_set1());
  //on_start_clicked();
}

pbd::qtmaindialog::~qtmaindialog()
{
  delete ui;
}

void pbd::qtmaindialog::display_l_table(const l_table& t)
{
  const auto& d = t.get();
  const int n_rows{static_cast<int>(d.size())};
  resize_table(n_rows);
  for (int i=0; i!=n_rows; ++i)
  {
    assert(i >=0 && i < static_cast<int>(d.size()));
    const auto& row = d[i];
    assert(ui->l_table->item(i, 0));
    ui->l_table->item(i, 0)->setText(QString::number(row.m_incip_lvl_lbl));
    ui->l_table->item(i, 1)->setText(QString::number(row.m_incip_lvl_lbl_par));
    ui->l_table->item(i, 2)->setText(QString::number(row.m_t_incipient));
    ui->l_table->item(i, 3)->setText(QString::number(row.m_t_good));
    ui->l_table->item(i, 4)->setText(QString::number(row.m_t_extinction));
    ui->l_table->item(i, 5)->setText(QString::number(row.m_species_level_label));
  }
}

void pbd::qtmaindialog::display_nltt_igtree_extinct(
  const nltt& points
)
{
  std::vector<double> xs = collect_nts(points);
  std::vector<double> ys = collect_nls(points);
  QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
  m_nltt_plot_igtree_extinct_line->setData(data);
  m_nltt_plot_igtree_extinct->replot();
}

void pbd::qtmaindialog::display_nltt_recon(
  const nltt& points
)
{
  std::vector<double> xs = collect_nts(points);
  std::vector<double> ys = collect_nls(points);
  QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
  m_nltt_plot_recontree_line->setData(data);
  m_nltt_plot_recontree->replot();
}

void pbd::qtmaindialog::display_png(const std::string& png_filename)
{
  QPixmap p;
  p.load(png_filename.c_str());
  ui->png->setPixmap(p);
}

pbd::parameters pbd::qtmaindialog::get_parameters() const noexcept
{
  const double birth_good{
    ui->parameters->item(row_birth_good,0)->text().toDouble()
  };
  const double birth_incipient{
    ui->parameters->item(row_birth_incipient,0)->text().toDouble()
  };
  const double completion{
    ui->parameters->item(row_completion,0)->text().toDouble()
  };
  const double death_good{
    ui->parameters->item(row_death_good,0)->text().toDouble()
  };
  const double death_incipient{
    ui->parameters->item(row_death_incipient,0)->text().toDouble()
  };
  const double time{
    ui->parameters->item(row_time,0)->text().toDouble()
  };
  const int seed{
    ui->parameters->item(row_seed,0)->text().toInt()
  };
  return parameters(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
}


void pbd::qtmaindialog::on_start_clicked()
{
  const std::string filename{"pbd_qtmaindialog_on_start_clicked.png"};
  pbd::sim_to_png(get_parameters(), filename);
  display_png(filename);
  display_l_table(
    pbd::sim_to_l_table(
      get_parameters()
    )
  );
  display_nltt_recon(
    pbd::sim_to_nltt_recon(
      get_parameters()
    )
  );
  display_nltt_igtree_extinct(
    pbd::sim_to_nltt_igtree_extinct_to_nltt(
      get_parameters()
    )
  );
}


void pbd::qtmaindialog::resize_table(const int n_rows)
{
  ui->l_table->setRowCount(n_rows);
  for (int row=0; row!=n_rows; ++row)
  {
    for (int col=0; col!=6; ++col)
    {
      if (!ui->l_table->item(row, col))
      {
        ui->l_table->setItem(row, col, new QTableWidgetItem);
      }
    }
  }
}


void pbd::qtmaindialog::on_button_load_clicked()
{
  try
  {
    const std::string filename
      = QFileDialog::getOpenFileName(
        this,
        "Open pbd::parameters file"
      ).toStdString();
    set_parameters(
      load_parameters(filename)
    );
    ui->label_sim_runtime->setText("File loaded successfully");
  }
  catch (std::exception &e)
  {
    ui->label_sim_runtime->setText(e.what());
  }

}

void pbd::qtmaindialog::set_parameters(const parameters& p)
{
  ui->parameters->item(row_birth_good,0)->setText(QString::number(p.get_birth_good()));
  ui->parameters->item(row_birth_incipient,0)->setText(QString::number(p.get_birth_incipient()));
  ui->parameters->item(row_completion,0)->setText(QString::number(p.get_completion()));
  ui->parameters->item(row_death_good,0)->setText(QString::number(p.get_death_good()));
  ui->parameters->item(row_death_incipient,0)->setText(QString::number(p.get_death_incipient()));
  ui->parameters->item(row_time,0)->setText(QString::number(p.get_time()));
  ui->parameters->item(row_seed,0)->setText(QString::number(p.get_seed()));
}
