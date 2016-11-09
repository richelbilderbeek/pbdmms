#include "pbd_qtmaindialog.h"

#include <cassert>
#include "pbd.h"
#include "pbd_l_table_row.h"
#include "ui_pbd_qtmaindialog.h"

pbd::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::pbd_qtmaindialog)
{
  ui->setupUi(this);
  on_start_clicked();
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

void pbd::qtmaindialog::display_png(const std::string& png_filename)
{
  QPixmap p;
  p.load(png_filename.c_str());
  ui->png->setPixmap(p);
}


void pbd::qtmaindialog::on_start_clicked()
{
  const double birth_good{
    ui->parameters->item(0,0)->text().toDouble()
  };
  const double birth_incipient{
    ui->parameters->item(1,0)->text().toDouble()
  };
  const double completion{
    ui->parameters->item(2,0)->text().toDouble()
  };
  const double death_good{
    ui->parameters->item(3,0)->text().toDouble()
  };
  const double death_incipient{
    ui->parameters->item(4,0)->text().toDouble()
  };
  const double time{
    ui->parameters->item(5,0)->text().toDouble()
  };
  const int seed{
    ui->parameters->item(6,0)->text().toInt()
  };
  const std::string filename{"pbd_qtmaindialog_on_start_clicked.png"};
  pbd::sim_to_png(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed,
    filename
  );
  display_png(filename);
  display_l_table(
    pbd::sim_to_l_table(
      birth_good,
      birth_incipient,
      completion,
      death_good,
      death_incipient,
      time,
      seed
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

